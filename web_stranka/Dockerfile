FROM node:slim

WORKDIR /app/

#skopíruje packa.json do priečinku, kde sa server nachádza
#je to tak spravené kvôli závislostiam
COPY server/package.json . 

RUN npm install

#skopíruje všetko
COPY server/ .

ENTRYPOINT ["npm", "start"]