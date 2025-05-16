import sounddevice as sd
import numpy as np
import time
import serial

# —— Audio & clap settings ——
SAMPLE_RATE         = 44100
DURATION            = 0.01    # 10 ms per block
HIGH_FREQ_CUTOFF    = 2000
MIN_HIGH_FREQ_RATIO = 0.2

THRESHOLD        = 0.2   # RMS volume to even consider
ATTACK_THRESHOLD = 0.3   # jump from previous RMS
MIN_ZCR          = 0.1
MIN_CENTROID     = 800

last_clap    = 0
prev_volume  = None

# —— Serial setup ——
# Change this to your port, e.g. '/dev/tty.usbmodem14101'
ser = serial.Serial('/dev/tty.usbserial-110', 9600)
time.sleep(2)  # give Arduino time to reset

print("Listening for claps…")

def callback(indata, frames, time_info, status):
    global last_clap, prev_volume

    # 1) RMS volume
    volume_norm = np.linalg.norm(indata)
    if volume_norm <= THRESHOLD:
        prev_volume = volume_norm
        return

    # 2) Attack‐time check
    if prev_volume is not None and (volume_norm - prev_volume) < ATTACK_THRESHOLD:
        prev_volume = volume_norm
        return
    prev_volume = volume_norm

    # 3) Spectral checks
    samples = indata[:, 0]
    fft_vals = np.abs(np.fft.rfft(samples))
    freqs    = np.fft.rfftfreq(len(samples), d=1/SAMPLE_RATE)
    total    = fft_vals.sum() + 1e-6

    # 3a) high‐freq ratio
    high_band = fft_vals[(freqs>=HIGH_FREQ_CUTOFF)&(freqs<=5000)].sum()
    if (high_band/total) < MIN_HIGH_FREQ_RATIO:
        return

    # 3b) zero‐crossing rate
    zcr = np.mean(np.abs(np.diff(np.sign(samples))))
    if zcr < MIN_ZCR:
        return

    # 3c) spectral centroid
    centroid = (freqs * fft_vals).sum() / total
    if centroid < MIN_CENTROID:
        return

    # 4) Debounce & report
    now = time.time()
    if now - last_clap > 0.2:
        last_clap = now
        vol_int = int(volume_norm * 100)  # scale to integer
        msg = f'C{vol_int}\n'
        ser.write(msg.encode())
        print(f"Sent to Arduino: {msg.strip()}")

# start audio stream
with sd.InputStream(callback=callback,
                    channels=1,
                    samplerate=SAMPLE_RATE,
                    blocksize=int(DURATION * SAMPLE_RATE)):
    while True:
        time.sleep(0.1)