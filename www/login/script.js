const loginForm = document.getElementById('loginForm');
loginForm.addEventListener('submit', function (e) {
    e.preventDefault();
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    fetch('/api/login', {  //API地址为/api/login
        method: 'POST',
        headers:{
            'Content-Type':'application/json'
        },
        body: JSON.stringify({username, password})
    })
    .then(response => {
        if (response.ok) {
            return response.json();
        } else {
            throw new Error('网络响应错误');
        }
    })
    .then(data => {
        // 处理后端返回的数据
        console.log(data);
        if (data.success) {
            alert('登录成功');
        } else {
            alert('登录失败');
        }
    })
    .catch(error=> {
        console.error('登录请求错误', error);
        alert('登录请求发生错误，请重试');
    });
});