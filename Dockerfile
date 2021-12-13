FROM gcc:4.9
COPY ./c /usr/src/myapp
WORKDIR /usr/src/myapp
RUN gcc -std=c99 -o lab6 lab6.c -lm
CMD ["./lab6"]