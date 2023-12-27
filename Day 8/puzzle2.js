
const fs = require('fs');
const { start } = require('repl');

class Node {

    constructor(name, left, right) {
    
        this.name = name;
        this.left = left;
        this.right = right;

    }

}

fs.readFile('map.txt', 'utf8', (err, data) => {

    if (err) {
        console.error(err);
        return;
    }

    let lines = data.split('\n');
    let directions = lines[0].trim();

    let nodes = [];
    for (let i = 2; i < lines.length; i++){

        let parts = lines[i].split(' ');
        let name = parts[0].trim();
        let left = parts[2].trim();
        let right = parts[3].trim();

        nodes.push(new Node(name, left.substring(1, left.length - 1), right.substring(0, right.length - 1)));

    }
    
    let startingNodes = [];
    for (let node of nodes){
        if (node.name[2] == "A"){
            startingNodes.push(node);
            console.log("Starting node: " + node.name );
        }
        
    }

    let steps = []

    for (let node of startingNodes){
    
        nodeSteps = 0;

        nodeEndsInZ = false;

        while(!nodeEndsInZ)

            for (let direction of directions){
            
                if (direction == 'L'){
                    node = nodes.find(n => n.name == node.left);
                    nodeSteps++;
                } else if (direction == 'R'){
                    node = nodes.find(n => n.name == node.right);
                    nodeSteps++;
                }

                if (node.name[2] == "Z"){
                    steps.push(nodeSteps);
                    nodeEndsInZ = true;
                    break;
                }
            }

    }

    let stepsNum = leasCommonMultiple(steps);

    // for (let node of startingNodes){
    //     console.log(node.name);
    // }

    // for (let line of lines) {
    //     console.log(line);
    // }

    console.log("Steps: " + stepsNum);



});

function leasCommonMultiple(numbers){
    
    function gcd(a, b) {
        return b ? gcd(b, a % b) : a;
    }

    function lcm(a, b) {
        return a * b / gcd(a, b);   
    }

    var multiple = numbers[0];
    numbers.forEach(function(n) {
        multiple = lcm(multiple, n);
    });

    return multiple;


}


