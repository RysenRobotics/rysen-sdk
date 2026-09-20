# Ubuntu 22.04 安装 Docker

### 1. 卸载可能冲突的旧版本

如果这是全新的系统，这一步即使提示没安装也没关系：

```bash
sudo apt remove -y docker.io docker-compose docker-compose-v2 docker-doc docker-buildx podman-docker containerd runc
```

### 2. 添加 Docker 官方源

整段复制执行：

```bash
sudo apt update

sudo apt install -y ca-certificates curl

sudo install -m 0755 -d /etc/apt/keyrings

sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg \
  -o /etc/apt/keyrings/docker.asc

sudo chmod a+r /etc/apt/keyrings/docker.asc

sudo tee /etc/apt/sources.list.d/docker.sources <<EOF
Types: deb
URIs: https://download.docker.com/linux/ubuntu
Suites: $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}")
Components: stable
Architectures: $(dpkg --print-architecture)
Signed-By: /etc/apt/keyrings/docker.asc
EOF

sudo apt update
```

这是目前 Docker 官方文档推荐的安装方式。

### 3. 安装 Docker

```bash
sudo apt install -y \
  docker-ce \
  docker-ce-cli \
  containerd.io \
  docker-buildx-plugin \
  docker-compose-plugin
```

这里已经包含：

```text
Docker Engine
Docker CLI
containerd
docker buildx
docker compose
```

所以以后直接可以使用：

```bash
docker compose
```

而不是老版本的：

```bash
docker-compose
```

### 4. 检查 Docker 是否启动

```bash
sudo systemctl status docker
```

如果看到：

```text
Active: active (running)
```

🎉 Docker 就装好了。

### 5. 推荐配置：以后不用每次输入 sudo

执行：

```bash
sudo usermod -aG docker $USER
```

然后**注销 Ubuntu 用户并重新登录**。

重新登录后测试：

```bash
docker ps
```

此时应该不需要：

```bash
sudo docker ...
```

最后可以检查版本：

```bash
docker --version
docker compose version
```
