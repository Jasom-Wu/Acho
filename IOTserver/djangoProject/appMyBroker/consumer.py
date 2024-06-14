import json
from asgiref.sync import async_to_sync
from channels.generic.websocket import WebsocketConsumer
from libs.token import checkDeviceToken, checkUserToken

deviceOnconnectLink = {}


class BaseConsumer(WebsocketConsumer):

    def connect(self):
        tok = self.scope['url_route']['kwargs']['token']
        operate = self.scope['url_route']['kwargs']['operate']
        receive_mode = self.scope['url_route']['kwargs']['receive_mode']
        temp_topic = self.scope['url_route']['kwargs']['topic']
        temp_topic_name = 'topic_%s' % temp_topic
        # which 'temp_topic' for device's: "<topic-name>" or for user's: "<topic-name>_id_<device-id>"
        res1, user = checkUserToken(tok)
        res2, device = checkDeviceToken(tok)
        self.sock_type = "unknown"
        self.accept()  # ought to accept connect first for a successful close-code sending
        if (res1 != 0 and res2 != 0) or operate not in {'subscribe', 'unsubscribe'} or \
                receive_mode not in {'any', 'only_device', 'only_user'}:
            self.close(code=3000)
            return
        if operate == 'subscribe':
            if res2 == 0:  # if a device connect just link its id as topic index
                temp_topic_name += f'_id_{device.id}'  # temp_topic_name: 'topic_<topic-name>_id_<device-id>'
                self.sock_type = 'device'  # store type of obj who launch the connection
                self.instance = device  # store obj who launch the connection
                if not deviceOnconnectLink.get(device.id):
                    deviceOnconnectLink[device.id] = [temp_topic_name]  # device connect, add to it's list for status
                else:
                    deviceOnconnectLink[device.id].append(temp_topic_name)
                if len(deviceOnconnectLink[device.id]) == 1:
                    device.status = 'online'
                    device.save()
            elif res1 == 0:  # if a user connection, it should check the 'device-id' is whether belonging to the user
                # or not
                mark = temp_topic_name.split('_')  # mark: [topic, <topic-name>, id, <device-id>]
                if len(mark) != 4 or mark[2] != 'id' or not mark[3] or not user.devices.filter(id=int(mark[3])).first():
                    self.close(code=3000)
                    return
                self.sock_type = 'user'
                self.instance = user

            self.receive_mode = receive_mode
            self.topic_name = temp_topic_name
            async_to_sync(self.channel_layer.group_add)(
                self.topic_name,
                self.channel_name
            )
        else:
            async_to_sync(self.channel_layer.group_discard)(
                self.topic_name,
                self.channel_name
            )
        text_json = json.dumps({'status': 'successful', 'operate': operate, 'topic': self.topic_name[6:]})
        self.send(text_json)

    def disconnect(self, close_code):
        if self.sock_type == 'device':
            try:
                deviceOnconnectLink[self.instance.id].remove(self.topic_name)
                self.instance.status = 'offline'
                self.instance.save()
            except ValueError as e:
                print(e)
        async_to_sync(self.channel_layer.group_discard)(
            self.topic_name,
            self.channel_name
        )

    def receive(self, text_data):
        forbid_charset = ['\r', '\n']
        temp = text_data
        for c in forbid_charset:
            temp = temp.replace(c, "")
        text_data_json = json.loads(temp)
        message = text_data_json['message']
        async_to_sync(self.channel_layer.group_send)(  # group send first and filter next
            self.topic_name,
            {
                'type': 'wrap_message',
                'message': message,
                'sock_type': self.sock_type,
                'instance': self.instance
            }
        )

    def wrap_message(self, event):
        message = event['message']
        sock_type = event['sock_type']
        instance = event['instance']
        trigger_name = 'Device2Device'
        if (instance == self.instance) or (self.receive_mode == 'only_device' and sock_type != 'device') or (
                self.receive_mode == 'only_user' and sock_type != 'user'):  # as a filter for msg category
            return

        if sock_type == 'user':
            trigger_name = instance.user_name
        elif sock_type == 'device' and self.sock_type == 'user':  # get device's nick_name sending to corresponding user
            trigger_name = instance.nick_name

        self.send(text_data=json.dumps({
            'source': trigger_name,
            'message': message
        }))
