import os

from django.shortcuts import render, HttpResponse
from django.http import JsonResponse
from libs.token import BindTokenGenerator, DeviceTokenGenerator
from appWelcome import models as user_model
import libs.token as TOK
from libs.token import user_token_check
from libs.token import user_token_checkAjax
from libs.token import device_token_check
from libs.dither import getQRCodeData

# Create your views here.
def test(request):
    if request.method == 'GET':
        device = user_model.Device.objects.filter(pk=1).first()
        return render(request, "home/device-panels/acho/acho_main.html", locals())


@user_token_check
def userHome(request, tok, user):
    temp = user.profile_pic
    if request.method == 'GET':
        return render(request, "home/home.html", locals())
    else:
        return render(request, "error500page/500.html")


@user_token_check
def devicePanels(request, tok, user):
    if request.method == 'GET':
        dev_id = request.GET.get('dev_id')
        if user:
            device = user.devices.filter(id=int(dev_id)).first()
            if not device:
                return render(request, "error500page/500.html")
            ur = "home/device-panels/%s/%s.html" % (device.type.type, f"{device.type.type}_main")
            return render(request, ur, locals())


@user_token_check
def userProfile(request, tok, user):
    if request.method == 'GET':
        if user.is_active == False:
            user.is_active = True
            user.save()
        return render(request, "home/app-pages/user-profile/user_profile.html", locals())


def deviceInit(request):
    if request.method == 'GET':
        device_mac = request.GET.get('mac')
        device_ip = request.GET.get('ip')
        device_type = request.GET.get('type')
        try_device = user_model.Device.objects.filter(mac=device_mac).first()
        type_instance = user_model.DeviceType.objects.filter(type=device_type).first()
        if not try_device:  # device is all new, mean that mac is not in database
            if device_type and type_instance:
                try_device = user_model.Device(mac=device_mac, ip=device_ip, type=type_instance)
            else:
                return JsonResponse({'token': '', 'status': 'failed'})
        if device_ip != "":
            try_device.ip = device_ip
            try_device.status = 'online'
            try_device.save()
            info = {
                'device_id': try_device.id,
                'device_ip': try_device.ip,
            }
            device_token = DeviceTokenGenerator.dumps(info).decode('utf-8')
            return JsonResponse({'token': device_token, 'status': 'successful'})
        else:
            return JsonResponse({'token': '', 'status': 'failed'})


@TOK.device_token_checkForFile
def deviceGetBindQRCode(request, json_data, device):
    if request.method == 'GET':
        size = request.GET.get("size")
        if device:
            info = {
                'id': device.id,
                'ip': device.ip,
            }
            bind_token = BindTokenGenerator.dumps(info).decode('utf-8')
            data = getQRCodeData(bind_token,eval(size))
            content_type = 'application/octet-stream'  # 默认情况下使用二进制流类型
            return HttpResponse(data, content_type=content_type)


@user_token_checkAjax
def userDeviceSetNickName(request, json_data, user):
    if request.method == 'GET':
        new_nick_name = request.GET.get('nick_name')
        device_id = request.GET.get('device_id')
        if new_nick_name and not new_nick_name[0].isdigit() and device_id.isdigit():
            if user:
                device = user.devices.filter(id=int(device_id)).first()
                if not device:
                    json_data['status'] = 'failed'
                    return JsonResponse(json_data)
                device = user_model.Device.objects.filter(id=device_id).first()
                device.nick_name = new_nick_name
                device.save()
            return JsonResponse(json_data)


@user_token_checkAjax
def userBind(request, json_data, user):  # return the result of binding ->json
    if request.method == 'GET':
        bind_tok = request.GET.get('bind_token')
        res2, device = TOK.checkBindToken(bind_tok)
        if res2 == 0:
            if not user.devices.filter(id=device.id).first():
                user.devices.add(device)  # link user and device by id
            else:
                json_data["status"] = "existed"
        else:
            json_data["status"] = "QR-expired"
        return JsonResponse(json_data)


@user_token_checkAjax
def userGetAuthority(request, json_data, user):
    if request.method == 'GET':
        authority = user.authority
        json_data['data'] = authority
        return JsonResponse(json_data)


@user_token_checkAjax
def userGetDeviceData(request, json_data, user):
    if request.method == 'GET':
        queryset = user.devices.all().values("id", "mac", "nick_name", "status", "type__type")
        for d in queryset:
            d['type'] = d.pop('type__type').title()
        device_dic_list = list(queryset)
        json_data['data'] = device_dic_list
        return JsonResponse(json_data)


def userGetDeviceProfileAvatar(request):
    if request.method == 'GET':
        id = request.GET.get("id")
        device_instance = user_model.Device.objects.filter(pk=id).first()
        img_file = device_instance.profile_pic
        data = img_file.file.read()
        image_extension = os.path.splitext(img_file.name)[1].lower()
        if image_extension == '.jpg' or image_extension == '.jpeg':
            content_type = 'image/jpeg'
        elif image_extension == '.png':
            content_type = 'image/png'
        elif image_extension == '.gif':
            content_type = 'image/gif'
        # 添加其他格式的判断
        else:
            content_type = 'application/octet-stream'  # 默认情况下使用二进制流类型
        return HttpResponse(data, content_type=content_type)


def showErrorPage(request):
    if request.method == 'GET':
        err = request.GET.get('type')
        if err == "expired":
            return render(request, "login_expired.html")
        else:
            return render(request, "error500page/500.html")


@user_token_checkAjax
def userProfileUploadAvatar(request, json_data, user):
    if request.method == "POST":
        file = request.FILES.get('file')
        if user:
            if user.profile_pic.name != "user-profile-avatar/default_user_profile_avatar.png":
                user.profile_pic.delete(False)
            user.profile_pic = file
            user.save()
        return JsonResponse(json_data)


@user_token_checkAjax
def userAvatarSetDefault(request, json_data, user):
    if request.method == "GET":
        if user:
            if user.profile_pic.name != "user-profile-avatar/default_user_profile_avatar.png":
                user.profile_pic.delete(True)
                user.profile_pic = "user-profile-avatar/default_user_profile_avatar.png"
                user.save()
            else:
                json_data['status'] = 'already'
        else:
            json_data['status'] = 'failed'
        return JsonResponse(json_data)
