# camera top
gst-launch-1.0 udpsrc port=5000 ! application/x-rtp,encoding-name=H264,payload=96 ! rtph264depay ! queue ! avdec_h264 ! autovideosink sync=false async=false -e

# camera front
#gst-launch-1.0 udpsrc port=5001 ! application/x-rtp,encoding-name=H264,payload=96 ! rtph264depay ! queue ! avdec_h264 ! autovideosink sync=false async=false -e
