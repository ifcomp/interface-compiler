# Web IPC

## Messages

### Call

```
var message =
[
	'call',
	<fully qualified canonical name of operation>,
	<uuid identifying the call>,
	[
		<handle to instance of class, if non-static>,
		<value of parameter>,
		...
	]
];
```

### Response

```
var message =
[
	'response',
	<fully qualified canonical name of operation>,
	<uuid identifying the associated call>,
	<value of result>,
	<null, or string of exception>
];
```

### Event

```
var message =
[
	'event',
	<fully qualified canonical canonical name of event>,
	<uuid identifying the event type>,
	[
		<value>,
		...
	]
];
```

## Values

### Primitives

| Canonical   | JSON                  | JavaScript            | Conversion |
|-------------|-----------------------|-----------------------|------------|
| Byte        | Number                | Number                | None       |
| UInt16      | Number                | Number                | None       |
| UInt32      | Number                | Number                | None       |
| UInt64      | Number                | Number                | None       |
| Boolean     | Boolean               | Boolean               | None       |
| Timestamp   | Number as Unix Time   | Date                  | Required   |
| String      | String                | String                | None       |
| Uuid        | Array of Number < 256 | Array of Number < 256 | None       |
| Buffer      | Array of Number < 256 | ArrayBuffer           | Required   |
| ConstBuffer | Array of Number < 256 | ArrayBuffer           | Required   |
| Vector      | Array                 | Array                 | None       |
| List        | Array                 | Array                 | None       |
| Set         | Array                 | Array                 | None       |
| Map         | Object                | Object                | None       |

### Model

| Canonical   | JSON                  | JavaScript            | Conversion |
|-------------|-----------------------|-----------------------|------------|
| Class       | Number as Handle      | Instance of Class     | Required   |
| Struct      | Object                | Instance of Struct    | Required   |
| Enum        | Number                | Number                | None       |
