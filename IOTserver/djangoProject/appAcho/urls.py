import os

from django.urls import path
from appAcho import views
from django.urls import re_path
from django.views.static import serve

from djangoProject.settings import BASE_DIR

urlpatterns = [
    path('home/image/', views.homeImage, name='homeImage'),
    path('home/audio/', views.homeAudio, name='homeAudio'),
    path('home/avatar_set/', views.avatarSet, name='avatarSet'),
    path('home/avatar_setdefault', views.avatarSetDefault, name='avatarSetDefault'),
    path('home/my_owners/', views.myOwners, name='myOwners'),
    path('home/upload_file/', views.uploadFile, name='uploadFile'),
    path('home/delete_file/', views.deleteFile, name='deleteFile'),
    path('device/get_dither_bin/', views.deviceGetDitherBin),
    path('device/get_file_list/', views.deviceGetFileList),
    path('device/get_audio_file/', views.deviceGetAudioFile),
    # re_path('upload/')
]
