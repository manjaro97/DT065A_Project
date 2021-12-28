import random

# Sensor class
class sensor():
    # Initialize mean, sigma, and starting value of sensor generation
    def __init__(self, mean, sigma, value):
        self.mean = mean
        self.sigma = sigma
        self.value = value

    # Get a new sensor data
    def get_sensor_data(self):

        # Get current value
        current_value = self.value
        new_value = self.value

        # Assign new value
        while new_value == current_value:
            new_value = int(random.gauss(self.mean, self.sigma))

        # Update & return new value 
        self.value = new_value
        new_value = str(new_value)
        return new_value.encode('utf-8')

# Test that sensor data generation works
if __name__ == '__main__':
    sens = sensor(mean=15, sigma=5, value=20)
    for _ in range(100):
        print(sens.get_sensor_data())