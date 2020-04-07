function webSocket(){
    let socket = new WebSocket("ws:127.0.0.1:8000")
    
    console.log("OUI")

    socket.onopen = function(e){
        alert("ok")
        let text_area = document.getElementById("text")
        document.getElementById("send-message").addEventListener("click", () => {
            console.log("sent")
            socket.send(text_area.value)
        })
    }

    socket.onmessage = function(event) {
        alert("test")
    }

    socket.onclose = function(){
        console.log("closed")
    }
}




function main(){
    webSocket()
}


window.onload = main
