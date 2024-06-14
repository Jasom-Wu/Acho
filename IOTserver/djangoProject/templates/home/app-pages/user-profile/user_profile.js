$(".user-profile-head-avatar-hover").click(function () {
            $(".user-avatar-upload-modal")[0].style.display = "inline";
        })
        $(".modal-inner-close-btn").click(function () {
            $(".user-avatar-upload-modal")[0].style.display = "none";
        })

        $("#avatar-default-btn").click(function () {
            $.ajax({
                url:`/home/user/avatar_setdefault/?tok=${window.sessionStorage.getItem('token_key')}`,
            type: 'GET',
            success: function (args) {
                setTimeout(function () {
                    checkToken(args['token']);
                    if(args['status']==='successful')
                    {
                        alert("已重置")
                        location.reload();
                    }
                    else if(args['status']==='already'){
                        alert("已经是默认了呢，亲~")
                    }
                    else
                        alert("重置失败！")
                    hideLoadingModal();
                },1000)
            }
            })
        })

        const dropArea = document.getElementById('modal-inner-upload-box');
        const allowedTypes = ['image/jpeg', 'image/png'];
        // 阻止浏览器默认行为，使拖拽不会导致新页面打开
        ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
            dropArea.addEventListener(eventName, function (e) {
                e.preventDefault();
                e.stopPropagation();
            });
        });

        // 拖拽进入区域时的样式
        ['dragenter', 'dragover'].forEach(eventName => {
            dropArea.addEventListener(eventName, function () {
                dropArea.classList.add('modal-inner-highlight');
                dropArea.getElementsByTagName("p")[0].innerText = "释放鼠标即可提交";
            });
        });

        // 拖拽离开区域时的样式
        ['dragleave', 'drop'].forEach(eventName => {
            dropArea.addEventListener(eventName, function () {
                document.getElementById('modal-inner-upload-box').classList.remove('modal-inner-highlight');
                dropArea.getElementsByTagName("p")[0].innerText = "点击或拖入图片文件至此处";
            });
        });

        // 拖拽放置时的处理
        dropArea.addEventListener('drop', function (e) {
            //方法二,这种方法另辟蹊径,巧妙的运用了forEach函数,经实验可成功获取文件
            let files = [];
            [].forEach.call(e.dataTransfer.files, function (file) {
                files.push(file);
            }, false);
            console.log(files);
            if (files.length > 0) {
                const file = files[0];
                if (allowedTypes.includes(file.type)) {
                    showLoadingModal();
                    uploadFile(file);
                } else {
                    alert('不支持的文件类型！');
                }
            }
        });

        function uploadFile(file) {
            const xhr = new XMLHttpRequest();
            xhr.open('POST', '/home/user/upload_avatar/', true);
            xhr.contentType = false
            xhr.processData = false
            xhr.onreadystatechange = function () {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    let data = JSON.parse(xhr.responseText);
                    checkToken(data['token']);
                    hideLoadingModal();
                    console.log(data);
                    if (data['status']==='failed'){
                        alert("出现意外，上传失败！");
                    }
                    else{
                        alert("上传成功！");
                        location.reload();
                    }
                    $(".user-avatar-upload-modal")[0].style.display = "none";
                }
                else if(xhr.readyState === 4){
                    hideLoadingModal();
                    alert("上传失败，请检查网络环境！");
                    $(".user-avatar-upload-modal")[0].style.display = "none";
                }
            };
            const formData = new FormData();
            formData.append('file', file);
            formData.append('tok', sessionStorage.getItem('token_key'));
            xhr.send(formData);
        }
        function showLoadingModal() {
            document.getElementById("loadingModal").style.display = "flex";
        }

        // 隐藏模态框
        function hideLoadingModal() {
            document.getElementById("loadingModal").style.display = "none";
        }
