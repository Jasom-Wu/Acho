from django.urls import re_path

from . import consumer

websocket_urlpatterns = [
    re_path(r'broker/ws/(?P<operate>.*?)/(?P<topic>.*?)/(?P<receive_mode>.*?)/(?P<token>.*?)$',
            consumer.BaseConsumer.as_asgi()),
]
