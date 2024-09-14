# Database Service

## Overview
A tool for managing and checking database blob consistency.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/lucyferrabee/database-service.git
    cd database-service
    ```

2. Install dependencies:
    ```bash
    composer install
    ```

3. Set up your environment variables:
    Copy `.env.example` to `.env` and configure your database URLs:
    ```bash
    cp .env.example .env
    ```

## Usage

### Build and Start

1. Build the Docker containers:
    ```bash
    docker-compose build
    ```

2. Start the Docker containers:
    ```bash
    docker-compose up -d
    ```

### Run the project

- Enter docker container:
    ```bash
    docker exec -it symfony_php /bin/bash
    ```
- Check blob consistency:
    ```bash
    php bin/console app:check-blob-consistency
    ```
- Check blob consistency with more information:
    ```bash
    php bin/console app:check-blob-consistency --more-info
    ```
- Run tests:
    ```bash
    ./vendor/bin/phpunit
    ```
  
## Development

- **Future Improvements**:

## Indexing


