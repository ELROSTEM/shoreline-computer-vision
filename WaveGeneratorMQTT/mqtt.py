import streamlit as st
import paho.mqtt.client as mqtt
import time

def wavetype_coversion(wavetype):
    if type(wavetype) == str:
        if wavetype == 'None':
            return  0
        elif wavetype == 'MonoPulse':
            return 1
        elif wavetype == 'wavetype2':
            return 2
        elif wavetype == 'wavetype3':
            return 3
    else:
        if wavetype == 0:
            return "None"
        elif wavetype == 1:
            return "MonoPulse"
        elif wavetype == 2:
            return "wavetype2"
        elif wavetype == 3:
            return "wavetype3"

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

wavetype = "None"
speed = 0
distance = 0

st.header("Dashboard")

with st.form("update_form"):
    wavetype = st.selectbox(
        'Wavetype:',
        ('None','MonoPulse','wavetype2', 'wavetype3'))
    
    speed = st.slider('Speed:', 0, 5000, 1000)
    distance = st.slider('Distance:', 0, 200, 100)
   
    submitted = st.form_submit_button("Update and run")
    if submitted:
        wavetype = wavetype_coversion(wavetype)
        client.connect("test.mosquitto.org",1883,60)
        client.publish("wave/motor", f"{wavetype} {speed} {distance}")
        client.disconnect()
        st.success("Running!")
        time.sleep(1)
    

st.subheader("Current Status:")
col1, col2, col3 = st.columns(3)
col1.metric("Wavetype", wavetype_coversion(wavetype))
col2.metric("Speed", speed)
col3.metric("Distance", distance)
