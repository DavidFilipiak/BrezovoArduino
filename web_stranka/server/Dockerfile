FROM node:slim

WORKDIR /app/

#skopíruje packa.json do priečinku, kde sa server nachádza
#je to tak spravené kvôli závislostiam
COPY package.json . 

RUN npm install

#skopíruje všetko
COPY . .

ENTRYPOINT ["npm" "start"]