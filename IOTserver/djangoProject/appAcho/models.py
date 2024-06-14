import os

from django import forms
from django.db import models

# Create your models here.
from appWelcome.models import Device


class UploadFileForm(forms.Form):
    file = forms.FileField(label='Select a file', required=True)
    description = forms.CharField(widget=forms.Textarea, max_length=200, required=False)
    type = forms.IntegerField(required=True, min_value=0, max_value=2)
    device_id = forms.IntegerField(required=True)


def Dynamic_upload_path(instance, filename):
    device_instance = instance.device
    upload_dir = 'device\\Acho\\{}\\{}'.format(device_instance.id, instance.get_type_display().lower())
    temp = os.path.join(upload_dir, filename)
    return temp


class UploadedFile(models.Model):
    type_choice = [
        (0, 'OTHERS'),
        (1, 'IMG'),
        (2, 'AUDIO')
    ]
    device = models.ForeignKey(to=Device, on_delete=models.CASCADE, null=True)
    file = models.FileField(upload_to=Dynamic_upload_path)  # 设置上传路径
    description = models.TextField()
    type = models.IntegerField(choices=type_choice, null=False)
    uploaded_at = models.DateTimeField(auto_now_add=True)
