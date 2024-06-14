let token_flush_counter = 0;
let devices_list = [];
let index;

function checkStorageSupport() {
    // sessionStorage
    return !!window.sessionStorage;
}

function getToken(username) {
    let xmlHttp = new XMLHttpRequest();  //创建一个异步请求对象
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState === 4 && xmlHttp.status === 200) {
            let new_token = xmlHttp.responseText;
            if (checkStorageSupport() === true) {
                sessionStorage.setItem('token_key', new_token);
                console.log(new_token);
                alert(new_token)
            }
        }
    };
    xmlHttp.open("get", "/welcome/user/get_token?user=" + username, true);
    xmlHttp.send();  //通知异步请求对象代替浏览器发送请求协议包
}

function checkToken(token) {
    if (token === 'expired') {
        window.location.replace('/home/err/?type=expired');
    } else if (token === 'error') {
        window.location.replace('/home/err/?type=500');
    } else {
        window.sessionStorage.setItem('token_key', token);
    }
}

function createDevNode(dev_nav, dev_info) {
    let dev_li = document.createElement("li");

    let dev_div = document.createElement("div");
    dev_div.setAttribute("class", "device_card");

    let dev_title = document.createElement("div");
    dev_title.setAttribute("class", "non-selectable");
    dev_title.appendChild(document.createTextNode(dev_info.nick_name))
    dev_div.appendChild(dev_title);

    let dev_img = document.createElement("img");
    let imageUrl = `/home/user/device_profile_avatar/?id=${dev_info.id}`;

    let xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", imageUrl, true);
    xmlhttp.responseType = "blob";
    xmlhttp.onload = function () {//不要用ajax！
        console.log(this);
        if (this.status === 200) {
            let blob = this.response;
            dev_img.onload = function (e) {
                window.URL.revokeObjectURL(dev_img.src);
            };
            dev_img.src = window.URL.createObjectURL(blob);
        }
    }

    xmlhttp.send();
    dev_div.appendChild(dev_img);

    let dev_p1 = document.createElement("p");
    let div_p1_strong = document.createElement("strong");
    dev_p1.setAttribute("class", "non-selectable");
    div_p1_strong.appendChild(document.createTextNode(dev_info.type));
    dev_p1.appendChild(div_p1_strong);
    dev_div.appendChild(dev_p1);

    let dev_p2 = document.createElement("p");
    dev_p2.setAttribute("class", "non-selectable");
    dev_p2.style.fontWeight = "Bold"
    dev_p2.appendChild(document.createTextNode("MAC  " + dev_info.mac));
    dev_div.appendChild(dev_p2);

    let dev_br = document.createElement("hr");
    dev_br.setAttribute("style", "margin: 10px 0 0;");
    dev_div.appendChild(dev_br);

    let dev_span = document.createElement("span");
    if (dev_info.status === 'offline')
        dev_span.setAttribute("class", "iconfont icon-cloud-offline-outline");
    else if (dev_info.status === 'online')
        dev_span.setAttribute("class", "iconfont icon-connection-line");
    dev_div.appendChild(dev_span);
    dev_li.appendChild(dev_div);
    return dev_li;
}

function showDevices() {
    let xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState === 4 && xmlHttp.status === 200) {
            let rec_data = xmlHttp.responseText;
            console.log(rec_data);
            let dic = JSON.parse(rec_data);
            checkToken(dic.token);
            console.log(dic.data);
            devices_list = dic.data;
            let main_board = document.getElementById('main_board');
            while (main_board.firstChild) {
                main_board.removeChild(main_board.firstChild);
            }
            let dev_nav = document.createElement("ul");
            dev_nav.setAttribute("id", 'devices_nav');
            main_board.appendChild(dev_nav);

            for (let i = 0; i < dic.data.length; i++) {
                let dev_li = createDevNode(dev_nav, dic.data[i]);
                dev_li.setAttribute('dev_index', i.toString());
                dev_nav.appendChild(dev_li);
                setTimeout(function () {
                    dev_li.style.opacity = "1";
                    dev_li.style.transform = "translateY(0)";
                }, 20);
            }
            dev_nav.addEventListener("click", function (event) {
                if (event.target.tagName !== "UL") {
                    let li_instance = event.target.closest('li');
                    index = parseInt(li_instance.getAttribute('dev_index'));
                    const siblings = Array.from(li_instance.parentNode.childNodes).filter(node => node !== li_instance && node.nodeType === 1);
                    siblings.forEach(function (item) {
                        item.style.opacity = "0";
                    })
                    setTimeout(function () {
                        li_instance.style.transform = "scale(95%)";
                        li_instance.style.opacity = "0";
                    }, 100);

                    setTimeout(function () {
                        showDevicePanel();
                    }, 1000);

                    // 在这里编写处理子元素点击事件的逻辑
                    console.log("子元素被点击了:", index);
                }
            });
        }
    };
    let token = window.sessionStorage.getItem('token_key');
    xmlHttp.open("get", "/home/user/getdevices_data/?tok=" + token, true);
    xmlHttp.send();
}


function showDevicePanel() {
    window.location.replace(`/home/user-home/dev_panels/?dev_id=${devices_list[index].id}&tok=${sessionStorage.getItem('token_key')}`);
}

function avatarClick(){
    window.location.replace(`/home/user/profile/?tok=${sessionStorage.getItem('token_key')}`);
}

