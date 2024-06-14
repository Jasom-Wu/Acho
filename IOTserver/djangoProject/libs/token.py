from django.http import JsonResponse
from django.shortcuts import render
from itsdangerous import TimedJSONWebSignatureSerializer as Serial
import itsdangerous
from appWelcome import models
from django.http import HttpResponseForbidden

# Create your views here.
user_token_salt = 'wuchunye-wuchunxi'
device_token_salt = 'boby123'
bind_token_salt = 'forever!'
UserTokenGenerator = Serial(user_token_salt, expires_in=6000)  # 过期时间10分钟
DeviceTokenGenerator = Serial(device_token_salt, expires_in=36000)  # 过期时间10h
BindTokenGenerator = Serial(bind_token_salt, expires_in=60)  # 过期时间60秒


def updateToken(token, salt, expire_time=2000, ):
    temp_generator = Serial(salt, expires_in=expire_time)
    res = temp_generator.loads(token.encode('utf-8'))
    return temp_generator.dumps(res).decode('utf-8')


def checkUserToken(token):  # 0表示Token有效；1表示Token过期；2表示Token错误
    activate_user = None
    try:
        res = UserTokenGenerator.loads(token.encode('utf-8'))
        activate_user = models.User.objects.get(id=res['user_id'])
        if activate_user:
            activate_user.is_active = True
            return 0, activate_user
        else:
            return 2, None
    except (itsdangerous.SignatureExpired, itsdangerous.BadSignature) as e:
        return (1, None) if isinstance(e, itsdangerous.SignatureExpired) else (2, None)
    finally:
        if activate_user:
            activate_user.save()


def checkBindToken(token):  # 0表示Token有效；1表示Token过期；2表示Token错误
    try:
        res = BindTokenGenerator.loads(token.encode('utf-8'))
        activate_device = models.Device.objects.get(id=res['id'])
        if activate_device:
            return 0, activate_device
        else:
            return 1, None
    except (itsdangerous.SignatureExpired, itsdangerous.BadSignature) as e:
        return (1, None) if isinstance(e, itsdangerous.SignatureExpired) else (2, None)


def checkDeviceToken(token):  # 0表示Token有效；1表示Token过期；2表示Token错误
    try:
        res = DeviceTokenGenerator.loads(token.encode('utf-8'))
        activate_device = models.Device.objects.filter(id=res['device_id']).first()
        if activate_device:
            return 0, activate_device
        else:
            return 1, None
    except (itsdangerous.SignatureExpired, itsdangerous.BadSignature) as e:
        return (1, None) if isinstance(e, itsdangerous.SignatureExpired) else (2, None)


# decorator for user token

def user_token_check(func):
    def inner(request, *args, **kwargs):
        if request.method == 'GET':
            tok = request.GET.get("tok")
            if not tok:
                return render(request, "error500page/500.html")
            res, user = checkUserToken(tok)
            if res == 0:
                tok = updateToken(tok, user_token_salt)
                return func(request, tok, user, *args, **kwargs)
            elif res == 1:
                return render(request, "login_expired.html")
            else:
                return render(request, "error500page/500.html")

    return inner


def user_token_checkAjax(func):
    def inner(request, *args, **kwargs):
        if request.method == 'POST':
            tok = request.POST.get("tok")
        else:
            tok = request.GET.get("tok")
        res, user = checkUserToken(tok)
        if res == 0:
            tok = updateToken(tok, user_token_salt)
            return func(request, {'status': 'successful', 'token': tok}, user, *args, **kwargs)
        elif res == 1:
            return JsonResponse({'status': 'failed', 'token': 'expired'})
        else:
            return JsonResponse({'status': 'failed', 'token': 'error'})

    return inner


# decorator for device token
def device_token_check(func):
    def inner(request, *args, **kwargs):
        if request.method == 'POST':
            tok = request.POST.get("tok")
        else:
            tok = request.GET.get("tok")
        res, device = checkDeviceToken(tok)
        if res == 0:
            tok = updateToken(tok, device_token_salt)
            return func(request, {'status': 'successful', 'token': tok}, device, *args, **kwargs)
        elif res == 1:
            return JsonResponse({'status': 'failed', 'token': 'expired'})
        else:
            return JsonResponse({'status': 'failed', 'token': 'error'})

    return inner


def device_token_checkForFile(func):
    def inner(request, *args, **kwargs):
        if request.method == 'POST':
            tok = request.POST.get("tok")
        else:
            tok = request.GET.get("tok")
        res, device = checkDeviceToken(tok)
        if res == 0:
            tok = updateToken(tok, device_token_salt)
            return func(request, {'status': 'successful', 'token': tok}, device, *args, **kwargs)
        elif res == 1:
            return HttpResponseForbidden("Forbidden")
        else:
            return HttpResponseForbidden("Forbidden")

    return inner
