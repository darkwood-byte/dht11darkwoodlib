#ifndef DHT11DARKWOODLIB_H
#define DHT11DARKWOODLIB_H

#define DHT_PIN 28

void dht11_start_signal() {
    gpio_init(DHT_PIN);
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(20); // Houdt de lijn laag voor 20 ms
    gpio_put(DHT_PIN, 1);
    sleep_us(30); // Houdt de lijn hoog voor 30 µs
    gpio_set_dir(DHT_PIN, GPIO_IN);
}

bool dht11_check_response() {
    sleep_us(40);
    if (!gpio_get(DHT_PIN)) {
        sleep_us(80);
        if (gpio_get(DHT_PIN)) {
            while (gpio_get(DHT_PIN));
            return true;
        }
    }
    return false;
}

uint8_t dht11_read_byte() {
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        while (!gpio_get(DHT_PIN)); // Wacht tot de lijn hoog wordt
        sleep_us(30);
        if (gpio_get(DHT_PIN))
            data = (data << 1) | 1; // Ontvangen '1'
        else
            data = (data << 1); // Ontvangen '0'
        while (gpio_get(DHT_PIN)); // Wacht tot de lijn laag wordt
    }
    return data;
}

bool dht11_read(int *temperature, int *humidity) {
    uint8_t data[5] = {0};
    dht11_start_signal();
    if (dht11_check_response()) {
        for (int i = 0; i < 5; i++) {
            data[i] = dht11_read_byte();
        }
        uint8_t checksum = data[0] + data[1] + data[2] + data[3];
        if (checksum == data[4]) {
            *humidity = data[0];
            *temperature = data[2];
            return true;
        }
    }
    return false;
}

#endif // DHT11HUGO_H
