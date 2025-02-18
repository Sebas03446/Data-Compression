from fastapi import APIRouter, WebSocket
from serial import Serial
import asyncio
from fastapi.responses import HTMLResponse
import json
from app.config import settings
from app.core.compression import decode_message

raw_data = []           
compressed_data = []    
reconstructed_data = [] 

router = APIRouter()

@router.get("/")
async def get():
    with open(r"index.html", "r") as f:
        return HTMLResponse(content=f.read())

@router.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    global reconstructed_data   
    await websocket.accept()    
    ser = None
    try:
        ser = Serial(settings.SERIAL_PORT, settings.BAUD_RATE, timeout=settings.SERIAL_TIMEOUT) 
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()   
                if line:
                    data = json.loads(line)
                    decoded_data = decode_message(data)
                    raw_data.append(decoded_data["raw_data"])
                    compressed_data.append(decoded_data["compressed_data"])
                    reconstructed_data = decoded_data["reconstructed_data"]

                    await websocket.send_json({
                        "raw_data": raw_data,                       
                        "compressed_data": compressed_data,         
                        "reconstructed_data": reconstructed_data,   
                        "compression_ratio": decoded_data["compression_ratio"]      
                    })     

                    await asyncio.sleep(0.1)       
    except Exception as e:
        print(f"Error reading from serial port: {str(e)}")
    finally:
        if ser:
            ser.close()
            print("Serial port closed")