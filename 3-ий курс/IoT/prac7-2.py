import time
import paho.mqtt.client as mqtt
import json
from datetime import datetime

import pandas as pd
import xml.etree.ElementTree as ET


# Параметры подключения к MQTT-брокеру
HOST = "192.168.2.24" # IP чемодана
PORT = 1883 # Стандартный порт подключения для Mosquitto
KEEPALIVE = 60 # Время ожидания доставки сообщения, если при отправке оно будет прeвышено, брокер будет считаться недоступным

# Словарь с топиками и собираемыми из них параметрами
SUB_TOPICS = {
    '/devices/wb-msw-v3_21/controls/CO2': 'co2',
    '/devices/wb-ms-11/controls/Illuminance': 'illuminance',
    '/devices/wb-m1w2-14/External Sensor 2': 'temperature',
    '/devices/wb-msw-v3_21/controls/Sound Level': 'sound',
    '/devices/wb-map12e_23/controls/Urms L1': 'voltage',
}

JSON_LIST = []

# Создание словаря для хранения данных JSON
JSON_DICT = {}
for value in SUB_TOPICS.values():
    JSON_DICT[value] = 0


def on_connect(client, userdata, flags, rc):
    """ Функция, вызываемая при подключении к брокеру

    Arguments:
    client - Экземпляр класса Client, управляющий подключением к брокеру
    userdata - Приватные данные пользователя, передаваемые при подключениии
    flags - Флаги ответа, возвращаемые брокером
    rc - Результат подключения, если 0, всё хорошо, в противном случае идем в документацию
    """
    print("Connected with result code " + str(rc))

    # Подключение ко всем заданным выше топикам
    for topic in SUB_TOPICS.keys():
        client.subscribe(topic)


last_check_time = time.time()

def on_message(client, userdata, msg):
    global last_check_time
    """ Функция, вызываемая при получении сообщения от брокера по одному из отслеживаемых топиков

    Arguments:
    client - Экземпляр класса Client, управляющий подключением к брокеру
    userdata - Приватные данные пользователя, передаваемые при подключениии
    msg - Сообщение, приходящее от брокера, со всей информацией
    """
    payload = msg.payload.decode() # Основное значение, приходящее в сообщение, например, показатель температуры
    topic = msg.topic # Топик, из которого пришло сообщение, поскольку функция обрабатывает сообщения из всех топиков

    param_name = SUB_TOPICS[topic]
    JSON_DICT[param_name] = float(payload)

    if(time.time() - last_check_time >= 5):
        JSON_DICT['host'] = HOST[-2:] 
        JSON_DICT['time'] = str(datetime.now())
        JSON_LIST.append(JSON_DICT.copy())

        pd.DataFrame(JSON_LIST).to_csv('/Users/student/Downloads/data.csv')

        with open('/Users/student/Downloads/data.json', 'w') as file:
            file.write(json.dumps(JSON_LIST))

        xml_root = ET.Element('data')
        for i in range(len(JSON_LIST)):
            entry = ET.SubElement(xml_root, 'entry')
            for k, v in JSON_LIST[i].items():
                ET.SubElement(entry, k).text = str(v)

        with open('/Users/student/Downloads/data.xml', 'w') as file:
            file.write(ET.tostring(xml_root, encoding='UTF-8', method='xml'))
        
        last_check_time = time.time()

def main():
    # Создание и настройка экземпляра класса Client для подключения в Mosquitto
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(HOST, PORT, KEEPALIVE)

    client.loop_forever() # Бесконечный внутренний цикл клиента в ожидании сообщений


if __name__ == "__main__":
    main()
