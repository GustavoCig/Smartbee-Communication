USE `smartbee_projeto`;


INSERT INTO users VALUES(1, 'teste','teste@teste','great','2629','9296','8D969EEF6ECAD3C29A3A629280E686CF0C3F5D5A86AFF3CA12020C923ADC6C92',1,0,NULL,NULL,NULL,NULL,NULL,NULL);

INSERT INTO apiario VALUES(1,'apiario 1','great','120','120','teste',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(1,'Temperatura','Medir temperatura interna da colmeia','DHT22','SparkFun Eletronic',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(2,'Umidade','Medir umidade interna da colmeia','DHT22','SparkFun Eletronic',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(3,'Tensão Sensores','Medir a tensão da bateria utilizada pelos sensores nas colmeias','0-25V(DC)','MH-Eletronic',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(4,'Tensão Repetidores','Medir tensão da bateria utilizada pelos repetidores','0-25V(DC)','MH--Eletronic',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(5,'Temperatura','Medir temperatura externa, esse sensor ficará posicionado do lado de fora do apiário e conectado ao sensor','DHT22','SparkFun Eletronic',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(6,'Umidade','Medir umidade externa, esse sensor ficará posicionado do lado de fora do apiário e conectado ao sensor','DHT22','SparkFun Eletronic',1,1,NULL,NULL,NULL);

INSERT INTO sensor VALUES(7,'MOdulo SIM','Sensor para enviar as amostras a nuvem utilizando tecnologia GPRS. É importante monitorar a tensão da bateria utilizada para monitorar esse sensor','SIM800L','SIM Com',1,1,NULL,NULL,NULL);

INSERT INTO colmeia_estado VALUES(1,'teste','.',1,1,NULL,NULL,NULL);

INSERT INTO colmeia_base VALUES(1,'teste','.',1,1,NULL,NULL,NULL);

INSERT INTO colmeia_origem VALUES(1,'teste','.',1,1,NULL,NULL,NULL);

INSERT INTO colmeia_tipo VALUES(1,'teste','.',1,1,NULL,NULL,NULL);

INSERT INTO colmeia_fundo VALUES(1,'teste','.',1,1,NULL,NULL,NULL);

INSERT INTO colmeia VALUES(1,'teste',1,'120','120',1,1,1,1,1,1,1,NULL,NULL,NULL);

