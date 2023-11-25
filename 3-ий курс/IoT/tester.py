import json
import os
import xml.etree.ElementTree as ET

os.system('clear')

with open('/Users/magomeddaurbekov/Desktop/ВУЗ/3-ий курс/IoT/data.json', 'r') as file:
    d = json.load(file)
    
    xml_root = ET.Element('data')
    for i in range(len(d)):
        entry = ET.SubElement(xml_root, 'entry')
        for k, v in d[i].items():
            ET.SubElement(entry, k).text = str(v)

    with open('/Users/magomeddaurbekov/Desktop/ВУЗ/3-ий курс/IoT/data.xml', 'w') as file:
        file.write(ET.tostring(xml_root, encoding='UTF-8', method='xml').decode())