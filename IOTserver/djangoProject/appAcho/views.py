import os

from django.shortcuts import render, HttpResponse
from django.http import JsonResponse
from appWelcome import models as user_model
from appAcho import models
import libs.token as TOK
from libs.token import user_token_check
from libs.token import user_token_checkAjax
from libs.token import device_token_check
from libs.token import device_token_checkForFile
from django.http import HttpResponseForbidden
from libs import dither


# Create your views here.
@user_token_check
def homeImage(request, tok, user):
    if request.method == "GET":
        dev_id = request.GET.get('dev_id')
        device = user_model.Device.objects.filter(pk=dev_id).first()
        if device:
            img_file_queryset = models.UploadedFile.objects.filter(device=device, type=1)
            counts = len(img_file_queryset)
            return render(request, "home/device-panels/acho/acho_app_homeimage.html", locals())
        else:
            return render(request, "error500page/500.html")


@user_token_check
def homeAudio(request, tok, user):
    if request.method == "GET":
        dev_id = request.GET.get('dev_id')
        device = user_model.Device.objects.filter(pk=dev_id).first()
        if device:
            img_file_queryset = models.UploadedFile.objects.filter(device=device, type=2)
            counts = len(img_file_queryset)
            return render(request, "home/device-panels/acho/acho_app_homeaudio.html", locals())
        else:
            return render(request, "error500page/500.html")


@user_token_check
def avatarSet(request, tok, user):
    pass


@user_token_check
def avatarSetDefault(request, tok, user):
    pass


@user_token_check
def myOwners(request, tok, user):
    pass


@user_token_checkAjax
def uploadFile(request, json_data, user):
    if request.method == 'POST':
        form = models.UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            uploaded_file = form.cleaned_data['file']
            description = form.cleaned_data['description']
            file_type = form.cleaned_data['type']
            device_id = form.cleaned_data['device_id']
            if user:
                device_instance = user.devices.filter(id=int(device_id)).first()
                if not device_instance:
                    json_data['status'] = 'failed'
                else:
                    # device_instance = user_model.Device.objects.filter(pk=device_id).first()
                    new_file = models.UploadedFile(file=uploaded_file, description=description, type=file_type,
                                                   device=device_instance)
                    new_file.save()
            else:
                json_data['status'] = "failed"
        else:
            json_data['status'] = "failed"
        return JsonResponse(json_data)


@user_token_checkAjax
def deleteFile(request, json_data, user):
    if request.method == 'GET':
        device_id = request.GET.get('dev_id')
        file_id = request.GET.get('file_id')
        if user:
            device_instance = user.devices.filter(id=int(device_id)).first()
            if not device_instance:
                json_data['status'] = 'failed'
            else:
                file_instance = models.UploadedFile.objects.filter(pk=file_id).first()
                if not file_instance or file_instance.device_id != int(device_id):
                    json_data['status'] = 'failed'
                else:
                    file_instance.file.delete()
                    file_instance.delete()
        else:
            json_data['status'] = "failed"
    return JsonResponse(json_data)


@device_token_check
def deviceGetFileList(request, json_data, device):
    if request.method == 'GET':
        file_type = request.GET.get("file_type")
        if device and int(file_type) in [x[0] for x in models.UploadedFile.type_choice]:
            data = ""
            file_list = models.UploadedFile.objects.filter(device=device, type=int(file_type)).values("file",
                                                                                                      "description",
                                                                                                      "id")
            for f in file_list:
                f['file'] = os.path.split(f['file'])[1].lower()
                data = data + "{}\t{}\t{}\n".format(f['id'], f['file'], f['description'])
            json_data['data'] = data
        else:
            json_data['status'] = 'failed'
        return JsonResponse(json_data)


@device_token_checkForFile
def deviceGetDitherBin(request, json_data, device):
    if request.method == 'GET':
        file_id = request.GET.get("file_id")
        scr_size = request.GET.get("scr_size")
        img_file = models.UploadedFile.objects.filter(id=int(file_id)).first()
        if img_file and img_file.type == 1 and img_file.device_id == device.id:
            data = dither.getDitherImgBinData(img_file.file.path, eval(scr_size))
            content_type = 'application/octet-stream'  # 默认情况下使用二进制流类型
            return HttpResponse(data, content_type=content_type)
        else:
            return HttpResponseForbidden("Forbidden")


@device_token_checkForFile
def deviceGetAudioFile(request, json_data, device):
    if request.method == 'GET':
        file_id = request.GET.get("file_id")
        audio_file = models.UploadedFile.objects.filter(id=int(file_id)).first()
        if audio_file and audio_file.type == 2 and audio_file.device_id == device.id:
            data = audio_file.file.read()
            content_type = 'application/octet-stream'  # 默认情况下使用二进制流类型
            return HttpResponse(data, content_type=content_type)
        else:
            return HttpResponseForbidden("Forbidden")
