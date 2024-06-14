from django.urls import path
from appWelcome import views

urlpatterns = [
    path('register/', views.register),
    path('login/', views.login),
    path('user/check/', views.userCheck),
    path('password/check/', views.passwordCheck),
    path('hello/', views.hello)
]
