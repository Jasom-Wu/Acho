import os

from django.urls import path
from appHome import views
from django.urls import re_path
from django.views.static import serve

from djangoProject.settings import BASE_DIR

urlpatterns = [
    path('test/', views.test),
    path('err/', views.showErrorPage),
    path('user-home/', views.userHome),
    path('user-home/dev_panels/', views.devicePanels),
    path('user/profile/', views.userProfile),
    path('user/bind/', views.userBind),
    path('user/setdevice_nickname/', views.userDeviceSetNickName),
    path('user/getauthority/', views.userGetAuthority),

    path('user/getdevices_data/', views.userGetDeviceData),
    path('user/device_profile_avatar/', views.userGetDeviceProfileAvatar),
    path('user/upload_avatar/', views.userProfileUploadAvatar),
    path('user/avatar_setdefault/', views.userAvatarSetDefault),
    path('device/init/', views.deviceInit),
    path('device/getbindQRCode/', views.deviceGetBindQRCode),
]
