import { useState } from "react";

import logo from './logo.svg';
import './App.css';
import { GreeterClient } from './protos/helloworld_grpc_web_pb';
import { HelloRequest } from './protos/helloworld_pb';

const client = new GreeterClient("http://localhost:8080", null, null);

function App() {
  const [name, setName] = useState("Alex")
  const [messages, setMessages] = useState(null);

  const sayHelloFn = () => {
    const request = new HelloRequest();
    request.setName(name);
    client.sayHello(request, null, (err, response) => {
      if (err) {
        console.log(err);
      }
      setMessages(response.getMessage())
    })
  }
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <div className="App-form">
          <form>
            <input placeholder="Enter your name" type="text" defaultValue={name}
              onChange={(e) => setName(e.target.value)} />
          </form>
        </div>
        <button onClick={sayHelloFn}>Say Hello To Server</button>
        <div>{messages}</div>
      </header>
    </div>
  );
}

export default App;
