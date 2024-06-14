import os
import secrets

from django.db import models
from django import forms


# Create your models here.


class DeviceType(models.Model):
    id = models.AutoField(primary_key=True)
    type = models.CharField(max_length=20, null=False)


def deviceDynamic_profile_pic_path(instance, filename):
    image_extension = os.path.splitext(filename)[1].lower()
    random_urlsafe = secrets.token_urlsafe(16)
    new_filename = "{}_{}{}".format(random_urlsafe, instance.id, image_extension)
    profile_pic_dir = 'device-profile-avatar'
    temp = os.path.join(profile_pic_dir, new_filename)
    return temp


class Device(models.Model):
    id = models.AutoField(primary_key=True)
    mac = models.CharField(max_length=64, null=False)
    status = models.CharField(max_length=10, null=False, default='offline')
    ip = models.CharField(max_length=20)
    nick_name = models.CharField(max_length=30, default='Device')
    type = models.ForeignKey(to=DeviceType, on_delete=models.CASCADE, null=True)
    profile_pic = models.ImageField(upload_to=deviceDynamic_profile_pic_path, blank=True)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        # 如果 profile_pic 字段没有被指定值，使用动态函数生成默认值
        if not self.profile_pic:
            type_instance = self.type
            default_path = 'device-profile-avatar\\default_{}.jpg'.format(type_instance.type)
            self.profile_pic = default_path


class UserAuthority(models.Model):
    id = models.AutoField(primary_key=True)
    authority_level = models.CharField(max_length=20, null=False)


def userDynamic_profile_pic_path(instance, filename):
    image_extension = os.path.splitext(filename)[1].lower()
    random_urlsafe = secrets.token_urlsafe(16)
    new_filename = "{}_{}{}".format(random_urlsafe, instance.user_name, image_extension)
    profile_pic_dir = 'user-profile-avatar'
    temp = os.path.join(profile_pic_dir, new_filename)
    return temp


class User(models.Model):
    id = models.AutoField(primary_key=True)
    user_name = models.CharField(max_length=255, null=False)
    password = models.CharField(max_length=255, null=False)
    is_active = models.BooleanField(default=False)
    devices = models.ManyToManyField(to=Device)
    authority = models.ForeignKey(to=UserAuthority, on_delete=models.CASCADE, default=1)
    profile_pic = models.ImageField(upload_to=userDynamic_profile_pic_path, blank=True)
    register_date = models.DateTimeField(auto_now_add=True)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        # 如果 profile_pic 字段没有被指定值，使用动态函数生成默认值
        if not self.profile_pic:
            default_path = 'user-profile-avatar/default_user_profile_avatar.png'
            self.profile_pic = default_path



