# This script first runs 'make' to build the project.
# If the build is successful, it then executes the compiled program.

# Execute the 'make' command
make

# Check if the last command (make) was successful
if ($?) {
    Write-Host "Build successful. Running executable..."
    # Execute the program
    cd build/test
    ./main.exe
    cd ../..
} else {
    Write-Host "Build failed. Please check the output from 'make'."
}