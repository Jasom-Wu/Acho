import asyncio
import websockets
import requests
import json

ip = "192.168.43.29"
token = requests.get(url='http://' + ip + ':8787/welcome/user/get_token/', params={'username': 'Jasom-wu'})
if token.status_code == 200:
    print(token.text)


    # bind_tok = "eyJhbGciOiJIUzUxMiIsImlhdCI6MTY4ODc0NDI1NSwiZXhwIjoxNjg4NzQ0MzE1fQ.eyJkZXZpY2VfaWQiOjIsImRldmljZV9pcCI6IjE5Mi4xNjguMC4yMDAifQ.jvIuVLYXqOM8x4UQgbsA87FyuLic588ndnQA2H1fD6aSOjUysYm743f2cakPU9I4JtAOI28Yayu7mWB6tEA1aA"
    # respond = requests.get(url='http://192.168.0.194:8787/home/user/bind/',
    #                        params={'tok': token.text, 'bind_token': bind_tok})
    # if respond.status_code == 200:
    #     print(respond.text)

    def on_message(dict_msg):
        pass


    async def webSocketRecv(websocket, once=False):
        while True:
            response = await websocket.recv()
            print(f"Received: {response}")
            on_message(json.loads(response))
            if once:
                break


    async def webSocketSend(websocket):
        loop = asyncio.get_event_loop()
        while True:
            future = loop.run_in_executor(None, input, '> ')
            msg = await future
            data = json.dumps({"message": msg})
            if data:
                await websocket.send(data)
                print(f"Sent: {data}")


    async def main():
        async with websockets.connect(
                "ws://" + ip + ":8787/broker/ws/subscribe/test_id_6/only_device/" + token.text) as websocket:
            await webSocketRecv(websocket, True)
            tasks_list = [
                asyncio.create_task(webSocketRecv(websocket), name='recv'),
                asyncio.create_task(webSocketSend(websocket), name='send'),
            ]
            done, pending = await asyncio.wait(tasks_list)

    asyncio.run(main())
