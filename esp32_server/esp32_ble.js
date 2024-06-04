import mqtt from "mqtt";
import fs from "fs";
import axios from "axios";
import express from "express";

const getCommand = async (operation) => {
  let config = {
    method: 'get',
    maxBodyLength: Infinity,
    url: `https://api.rentlycore.com/oakslock/device/bleCommand?command=${operation}&lockMac=EA:DF:62:EA:BE:A5`,
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded',
      'Authorization': 'Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwYXJ0bmVySWQiOjIsImRldmljZU1hYyI6IkVBOkRGOjYyOkVBOkJFOkE1Iiwicm9sZSI6IkFETUlOIiwiaWF0IjoxNzE3NDg3NDU5LCJleHAiOjE3MTc1NzM4NTl9.KErNU06bo_1DqaTS04dqYba04_AcMI7-Z4dstC_NRg0'
    }
  };

  const { data = {} } = await axios.request(config);

  return data.command;

}

const endPoint = "a3oibjuw7emgs5-ats.iot.us-east-1.amazonaws.com"

const clientId = '12345-' + Date.now();

const options = {
  clientId: clientId,
  key: fs.readFileSync("/Users/madamanchisriharsha/Downloads/esp32_ble/privatekey.pem"),
  cert: fs.readFileSync("/Users/madamanchisriharsha/Downloads/esp32_ble/certificate.pem"),
  ca: fs.readFileSync("/Users/madamanchisriharsha/Downloads/esp32_ble/root.pem")
}

const client = mqtt.connect(`mqtts://${endPoint}`, options);

const publish = async (operation) => {
  const command = await getCommand(operation)
  client.publish("subscribe/esp32_ble", command, () => console.log(operation, "command published"))
}

client.on('connect', async () => {
  console.log('Connected to AWS IoT')
});


client.on('close', async () => {
  console.log('Connection closed')
});

const app = express()

const PORT = 3000

app.get('/api/lock', (req, res) => {
  publish("lock")
  res.send({ status: 200, message: `Success` });
})

app.get('/api/unlock', (req, res) => {
  publish("unlock")
  res.send({ status: 200, message: `Success` });
})

app.listen(PORT, () => {
  console.log(`server listening on port ${PORT}`)
})