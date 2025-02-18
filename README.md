# Data Compression with Delta Encoding & WebSocket Communication

This project captures color data using an Arduino Nano 33 BLE Sense Lite with the APDS9960 sensor and applies delta encoding to reduce the size of transmitted data. The data is read via a serial connection in Python and updated on a web page using FastAPI WebSocket.

## Installation Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/Sebas03446/Data-Compression.git
   cd Data-Compression
   ```

2. Create a virtual environment and activate it:
   ```bash
   python3 -m venv venv
   source venv/bin/activate  # On Windows use `venv\Scripts\activate`
   ```

3. Install the required dependencies:
   ```bash
   pip install -r requirements.txt
   ```

## Usage Instructions
1. Start the FastAPI server:
   ```bash
   fastapi run app/main.py
   ```

2. Open your web browser and navigate to `http://localhost:8000/api/v1/` to access the web interface.

3. Use the web interface to visualize the data compression process.

## Authors
- Lee Xin Yee
- Léo Vasseur
- Jean Sebastian French
