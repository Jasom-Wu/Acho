from django.shortcuts import render, HttpResponse
from django.http import JsonResponse
from appWelcome import models
from libs.token import UserTokenGenerator


def register(request):
    if request.method == 'POST':
        user_name = request.POST.get('username')
        password = request.POST.get('password')
        res = models.User.objects.create(password=password, user_name=user_name)
        if res:
            return render(request, 'register_success.html', locals())
    else:
        return render(request, 'register.html')


def login(request):
    if request.method == 'GET':
        return render(request, 'login.html')
    else:
        return render(request, "error500page/500.html")


def userCheck(request):  # 1表示存在，0表示不存在
    if request.method == 'GET':
        user_input_name = request.GET.get('username')
        try_user = models.User.objects.filter(user_name=user_input_name).first()
        res = {"status": "NoFound"}
        if try_user:
            res['status'] = "Found"
        return JsonResponse(res)


def passwordCheck(request):
    if request.method == 'GET':
        user_input_name = request.GET.get('username')
        user_input_password = request.GET.get('password')
        try_user = models.User.objects.filter(user_name=user_input_name).first()
        res = {"status": "invalid-user"}
        if try_user:
            if user_input_password == try_user.password:
                user_id = try_user.id
                info = {
                    'user_id': user_id,
                    'user_name': user_input_name,
                }
                token = UserTokenGenerator.dumps(info).decode('utf-8')
                try_user.is_active = True
                try_user.save()
                res["status"] = "corrected"
                res["token"] = token
            else:
                res["status"] = "uncorrected"
        return JsonResponse(res)


def hello(request):
    return render(request, 'login_expired.html')
