FROM python:3.7.6-stretch

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git build-essential wget bash ca-certificates

RUN rm /bin/sh && ln -s /bin/bash /bin/sh
RUN mkdir /usr/local/nvm
ENV NVM_DIR /usr/local/nvm
ENV NODE_VERSION 12.14.1

# Install nvm with node and npm
RUN wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.35.2/install.sh | bash

RUN source $NVM_DIR/nvm.sh && nvm install $NODE_VERSION \
    && nvm alias default $NODE_VERSION \
    && nvm use default

ENV NODE_PATH $NVM_DIR/v$NODE_VERSION/lib/node_modules
ENV PATH $NVM_DIR/versions/node/v$NODE_VERSION/bin:$PATH

RUN npm i npm@6.13.0 -g
RUN npm i pm2 -g
RUN npm i node-gyp -g

RUN mkdir -p /root/.ssh/
RUN chmod 0700 /root/.ssh

RUN ssh-keyscan github.com >/root/.ssh/known_hosts
RUN ssh-keyscan bitbucket.org >/root/.ssh/known_hosts

WORKDIR /app

COPY package.json /app/
COPY package-lock.json /app/

RUN npm ci

COPY tsconfig.json binding.gyp /app/

COPY src /app/src

RUN node-gyp rebuild && npm run tsc

ENTRYPOINT node src/main.js
