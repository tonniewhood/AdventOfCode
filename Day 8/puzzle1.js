
const fs = require('fs');

class Node {

    constructor(name, left, right) {
    
        this.name = name;
        this.left = left;
        this.right = right;
        this.endingLocation = "";

    }

}

fs.readFile('map.txt', 'utf8', (err, data) => {

    if (err) {
        console.error(err);
        return;
    }

    let lines = data.split('\n');
    let directions = lines[0];

    let nodes = [];
    for (let i = 2; i < lines.length; i++){

        let parts = lines[i].split(' ');
        let name = parts[0].trim();
        let left = parts[2].trim();
        let right = parts[3].trim();

        nodes.push(new Node(name, left.substring(1, left.length - 1), right.substring(0, right.length - 1)));

    }
    
    let currentNode = nodes.find(node => node.name == "AAA");
    let steps = 0;

    while (currentNode.name != "ZZZ"){

        for (let i = 0; i < directions.length; i++){

            if (directions.charAt(i) == 'L'){
                // console.log("For node: " + currentNode.name + " going left to: " + currentNode.left);
                currentNode = nodes.find(node => node.name == currentNode.left);
                steps++;

            } else if (directions.charAt(i) == 'R'){
                // console.log("For node: " + currentNode.name + " going right to: " + currentNode.right);
                currentNode = nodes.find(node => node.name == currentNode.right);
                steps++;
            }

            if (currentNode.name == "ZZZ"){

                break;

            }

            

        }

    }

    for (let line of lines) {
        console.log(line);
    }

    console.log("Steps: " + steps);



});
