FROM hello-world:lastest

RUN docker pull hello-world

CMD ["python"]
