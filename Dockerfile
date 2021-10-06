FROM postgres
ENV POSTGRES_PASSWORD postgres
ENV POSTGRES_DB testdb
COPY Database_scheme.sql /docker-entrypoint-initdb.d/
