FROM python:3.11-slim

WORKDIR /app

COPY gateway/ml_inference/requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY gateway/ml_inference/ /app/ml_inference/

RUN useradd -r -s /usr/sbin/nologin appuser

ENV MQTT_HOST=localhost
ENV MQTT_PORT=1883

USER appuser
CMD ["python", "-m", "ml_inference.detector"]
