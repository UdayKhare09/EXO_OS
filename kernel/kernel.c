// Basic kernel function
void main() {
    // Video memory base address
    char* video_memory = (char*) 0xb8000;

    // Message to display
    const char* message = "Hello from C kernel!";

    // Print message to screen
    int i = 0;
    while(message[i] != 0) {
        *video_memory = message[i];
        video_memory++;
        // Color attribute: white on black
        *video_memory = 0x0F;
        video_memory++;
        i++;
    }
}