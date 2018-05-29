FROM ubuntu:14.04
MAINTAINER Alexander Kutelev <alexander@kutelev.ru>
RUN apt-get update && apt-get -y upgrade; \
    echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true | debconf-set-selections; \
    apt-get install -y g++ make inkscape pdftk texlive-xetex texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended texlive-lang-cyrillic locales msttcorefonts; \
    apt-get install -y git wget; \
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
