def decode_message(data, keyframe_interval=3):
    raw = data.get("raw", {})
    delta = data.get("delta", {})

    def decode_channel(delta_channel):
        if not delta_channel:
            return []
        reconstructed = [delta_channel[0]]
        for i in range(1, len(delta_channel)):
            if i % keyframe_interval == 0:
                reconstructed.append(delta_channel[i])
            else:
                reconstructed.append(reconstructed[-1] + delta_channel[i])
        return reconstructed

    reconstructed_data = {}
    for channel, delta_channel in delta.items():
        reconstructed_data[channel] = decode_channel(delta_channel)

    if raw:
        sample_channel = next(iter(raw.values()))
        n = len(sample_channel)
    else:
        n = 0

    if n > 0:
        keyframes = 1 + (n - 1) // keyframe_interval
        raw_size = 3 * n * 2  
        compressed_size = 3 * ((n - keyframes) * 1 + keyframes * 2)
        compression_ratio = raw_size / compressed_size
    else:
        compression_ratio = 0

    return {
        "raw_data": raw,
        "compressed_data": delta,
        "reconstructed_data": reconstructed_data,
        "compression_ratio": compression_ratio
    }
    