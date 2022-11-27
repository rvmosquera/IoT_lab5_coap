FROM python:3.10
LABEL Maintainer="Wasabi@team"

#host for calling functions inside minikube
#ENV OPENFAAS_HOST=http://10.105.163.115:8080

WORKDIR /coap-app

COPY requeriments.txt .

RUN pip install --no-cache-dir -r requeriments.txt

COPY server.py server.py

# Default CoAP port
EXPOSE 5683

CMD ["python", "server.py"]