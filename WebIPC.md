# Web IPC

## Messages

### Call

```
var message = {
	type: 'call',
	id: <uuid identifying the call>,
	operation: <fully qualified canonical name of operation>,
	parameters: [
		<handle to instance of class, if non-static>
		<value of parameter>,
		...
	]
};
```

### Response

```
var message = {
	type: 'response',
	id: <uuid identifying the associated call>,
	result: <value of result>,
	exception: <null, or string>
};
```

### Event

```
var message = {
	type: 'event',
	id: <uuid identifying the event type>,
	class: <fully qualified canonical name of class>
	event: <canonical name of event>
	values: [
		<value>,
		...
	]
};
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
