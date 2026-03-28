FROM python:3.11-slim

WORKDIR /app

COPY gateway/cloud_connector/requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY gateway/cloud_connector/ /app/cloud_connector/

RUN useradd -r -s /usr/sbin/nologin appuser && chown -R appuser /app

ENV MQTT_HOST=localhost
ENV MQTT_PORT=1883
ENV AWS_IOT_ENDPOINT=""
ENV AWS_IOT_PORT=8883
ENV AWS_CERT_DIR=/certs
ENV DEVICE_ID=""

USER appuser
CMD ["python", "-m", "cloud_connector.main"]
