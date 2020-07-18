FROM ubuntu:20.04
LABEL maintainer="Alexander Kutelev <alexander@kutelev.ru>"
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade; \
    echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true | debconf-set-selections; \
    apt-get install -y tzdata; \
    echo "Asia/Tomsk" > /etc/timezone; \
    ln -fs /usr/share/zoneinfo/Asia/Tomsk /etc/localtime; \
    dpkg-reconfigure -f noninteractive tzdata; \
    apt-get install -y \
        cmake \
        g++ \
        git \
        inkscape \
        locales \
        msttcorefonts \
        pdftk \
        rename \
        texlive-fonts-recommended \
        texlive-lang-cyrillic \
        texlive-latex-extra \
        texlive-latex-recommended \
        texlive-xetex \
        wget; \
    locale-gen en_US.UTF-8; dpkg-reconfigure --frontend=noninteractive locales; update-locale LANG=en_US.UTF-8; \
    mkdir /root/.fonts; \
    cd /root/.fonts; \
    wget https://github.com/kutelev/gost_forms/blob/master/fonts/gost_a.ttf?raw=true -q -O gost_a.ttf; \
    wget https://github.com/kutelev/gost_forms/blob/master/fonts/gost_ab.ttf?raw=true -q -O gost_ab.ttf; \
    wget https://github.com/kutelev/gost_forms/blob/master/fonts/gost_abi.ttf?raw=true -q -O gost_abi.ttf; \
    wget https://github.com/kutelev/gost_forms/blob/master/fonts/gost_ai.ttf?raw=true -q -O gost_ai.ttf; \
    fc-cache -fv
ENV LC_ALL=en_US.UTF-8
WORKDIR "/root/workdir"
