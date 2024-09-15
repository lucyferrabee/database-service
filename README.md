# Database Service

## Overview
A tool for managing and checking database blob consistency across multiple tables by comparing it with a 'master' stored number.

This service aims to check inconsistencies between columns on different databases. It works based on two database connections,
which are passed to various Repository classes. Models are also included in this repo, but please note that this is just to exhibit
how this service would fit within a wider context of an enterprise product. Large queries are run in batches to improve efficiency.

The service is run on the command line, with an option for simple output as well as verbose output, using a 'more-info' parameter.

Please note: this is a POC and not a complete project, and is used to show how a service could work to check inconsistencies, with
the focus on using abstraction to allow for scalability, as well as keeping efficiency of queries. Please see below for more information
regarding improvements/thoughts around possible libraries to integrate and how to manage scaling up.

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
  
## Future Improvements

Testing
- Enhance Test Coverage: Expand unit and integration tests to cover all aspects of the codebase. This includes edge cases, error scenarios, and ensuring all business logic paths are tested.
- Use Mocking and Stubs: Utilize mocking libraries like PHPUnit’s built-in mocking capabilities to simulate different scenarios and isolate tests from external dependencies, such as database connections.
Efficiency
- Parallel Queries: Implement parallel processing for database queries to reduce overall execution time. Consider libraries like Swoole or extensions like pthreads for concurrent execution in PHP.
- Asynchronous Execution: Incorporate asynchronous processing using libraries like ReactPHP or Amp. These libraries facilitate non-blocking I/O operations, which can improve performance, especially for I/O-bound tasks.
- AWS Lambda: Leverage AWS Lambda functions for processing tasks in parallel. This serverless approach allows you to execute multiple functions concurrently, scaling with demand.
- Database Indexing: Create a script to index relevant database columns. Indexes significantly speed up query performance by reducing the amount of data scanned. Ensure to analyze the database schema and identify columns frequently used in queries for indexing.
Logging
- Use a Logging Tool: Replace direct command-line output with a robust logging tool. Integrate with services like Datadog, ELK Stack (Elasticsearch, Logstash, Kibana), or similar solutions for real-time monitoring and error tracking.
- Database Logging Table: Implement a logging mechanism that records events and errors in a dedicated database table. This will facilitate in-depth analysis and allow for the generation of dashboards or automated responses to specific conditions.
- Error Handling and Alerts: Enhance error handling to capture exceptions and log them appropriately. Configure alerts for critical issues to ensure prompt attention and resolution.
Frontend/Dashboard
- API Integration: Develop an API endpoint to expose results from the service. This allows integration with a frontend UI or other systems for real-time data access.
- User Interface: Create a user-friendly dashboard for operational members to visualize results and manage the data. This can include charts, tables, and other visualizations to represent the state of data consistency and processing status.
Additional Considerations
- Documentation: Ensure comprehensive documentation for both developers and users. This includes setup instructions, usage guidelines, and examples.
- Security: Review security practices, especially for database connections and sensitive information. Use environment variables for configuration and ensure they are not exposed in version control.
- Scalability: Design with scalability in mind. As data volume grows, ensure the system can handle increased load without performance degradation.