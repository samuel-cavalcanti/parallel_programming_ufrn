class Data {
  Data({required this.time});

  double time;

  void incrementTime(double deltaTime) {
    time += deltaTime;
  }
}

Stream<Data> fetch(Stream<Data> streamData) async* {
  var deltaTime = 2.0;
  await for (Data data in streamData) {
    data.incrementTime(deltaTime);
    yield data;
  }
}

Stream<Data> store(Stream<Data> streamData) async* {
  var deltaTime = 2.0;
  await for (Data data in streamData) {
    data.incrementTime(deltaTime);
    yield data;
  }
}

Stream<Data> anotherOperations(Stream<Data> streamData) async* {
  var deltaTime = 1.0;

  await for (Data data in streamData) {
    data.incrementTime(deltaTime);
    yield data;
  }
}

Stream<Data> unPipelined(List<Stream<Data> Function(Stream<Data>)> operations,
    Stream<Data> dataStream) async* {
  for (final operation in operations) {
    dataStream = operation(dataStream);
  }

  yield* dataStream;
}

pipelined(List<Stream<Data> Function(Stream<Data>)> operations,
    Stream<Data> dataStream) async {}

void main() async {
  final data = List.generate(1000, (index) => Data(time: 0.0));

  final operations = [
    fetch,
    anotherOperations, //1
    anotherOperations, //2
    anotherOperations, //3
    anotherOperations, //4
    anotherOperations, //5
    store,
  ];

  double time = 0.0;

  final dataStream = unPipelined(operations, Stream.fromIterable(data));

  final result = await dataStream.toList();

  final total =
      result.map((e) => e.time).reduce((value, element) => value + element);

  print('total $total');
}
