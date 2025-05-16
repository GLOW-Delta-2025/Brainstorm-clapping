import sounddevice as sd
import numpy as np
import time

SAMPLE_RATE = 44100
HIGH_FREQ_CUTOFF = 2000
MIN_HIGH_FREQ_RATIO = 0.2

THRESHOLD = 1.2  # minimum volume to detect a clap
DURATION = 0.01
last_clap = 0
intervals = []

ATTACK_THRESHOLD = 0.3  # minimum jump in energy to count as a clap
prev_volume = None

def callback(indata, frames, time_info, status):
    global last_clap, intervals, prev_volume
    volume_norm = np.linalg.norm(indata)  # RMS
    if volume_norm > THRESHOLD:
        # Attack-time check: require a fast energy rise
        if prev_volume is not None:
            if (volume_norm - prev_volume) < ATTACK_THRESHOLD:
                prev_volume = volume_norm
                return
        prev_volume = volume_norm
        # --- spectral band-pass check (2–5 kHz) ---
        samples = indata[:, 0]
        fft_vals = np.abs(np.fft.rfft(samples))
        freqs = np.fft.rfftfreq(len(samples), d=1/SAMPLE_RATE)
        high_band = np.sum(fft_vals[(freqs >= 2000) & (freqs <= 5000)])
        total = fft_vals.sum() + 1e-6
        if high_band / total < 0.1:
            return

        # --- zero-crossing rate check ---
        zcr = np.mean(np.abs(np.diff(np.sign(samples))))
        if zcr < 0.1:
            return

        # --- spectral centroid check ---
        centroid = np.sum(freqs * fft_vals) / total
        if centroid < 800:
            return
        now = time.time()
        if now - last_clap > 0.2:
            print(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - Clap! Volume: {volume_norm:.2f}")
            if last_clap != 0:
                interval = now - last_clap
                bpm = 60 / interval
                print(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - Interval: {interval:.2f}s ~{bpm:.0f} BPM")
                intervals.append(interval)
                if len(intervals) > 5:
                    intervals.pop(0)
                if len(intervals) >= 3:
                    avg = sum(intervals) / len(intervals)
                    max_dev = max(abs(i - avg) for i in intervals)
                    if max_dev < 0.1 * avg:
                        print(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - In sync (~{60 / avg:.0f} BPM)")
                    else:
                        print(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - Not steady")
            last_clap = now

print(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - Listening for claps... Press Ctrl+C to stop.")
with sd.InputStream(callback=callback, channels=1, samplerate=44100):
    while True:
        time.sleep(0.1)