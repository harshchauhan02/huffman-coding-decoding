class Node {
    constructor(character, freq) {
        this.character = character;
        this.freq = freq;
        this.left = null;
        this.right = null;
    }
}

class MinHeap {
    constructor() {
        this.heap = [];
    }

    insert(node) {
        this.heap.push(node);
        this.heapifyUp();
    }

    extractMin() {
        if (this.heap.length === 1) return this.heap.pop();
        const min = this.heap[0];
        this.heap[0] = this.heap.pop();
        this.heapifyDown(0);
        return min;
    }

    heapifyUp() {
        let index = this.heap.length - 1;
        while (index > 0) {
            let element = this.heap[index];
            let parentIndex = Math.floor((index - 1) / 2);
            let parent = this.heap[parentIndex];
            if (parent.freq <= element.freq) break;
            this.heap[index] = parent;
            this.heap[parentIndex] = element;
            index = parentIndex;
        }
    }

    heapifyDown(index) {
        let leftChildIndex = 2 * index + 1;
        let rightChildIndex = 2 * index + 2;
        let smallest = index;
        if (leftChildIndex < this.heap.length && this.heap[leftChildIndex].freq < this.heap[smallest].freq) {
            smallest = leftChildIndex;
        }
        if (rightChildIndex < this.heap.length && this.heap[rightChildIndex].freq < this.heap[smallest].freq) {
            smallest = rightChildIndex;
        }
        if (smallest !== index) {
            [this.heap[index], this.heap[smallest]] = [this.heap[smallest], this.heap[index]];
            this.heapifyDown(smallest);
        }
    }
}

function buildHuffmanTree(charFreq) {
    let minHeap = new MinHeap();
    for (let char in charFreq) {
        minHeap.insert(new Node(char, charFreq[char]));
    }
    while (minHeap.heap.length > 1) {
        let left = minHeap.extractMin();
        let right = minHeap.extractMin();
        let sum = new Node(null, left.freq + right.freq);
        sum.left = left;
        sum.right = right;
        minHeap.insert(sum);
    }
    return minHeap.extractMin();
}

function generateCodes(node, code, codes) {
    if (!node) return;
    if (node.character) {
        codes[node.character] = code;
    }
    generateCodes(node.left, code + '0', codes);
    generateCodes(node.right, code + '1', codes);
}

function generateHuffmanTree() {
    const inputText = document.getElementById('inputText').value;
    const charFreq = {};
    for (let char of inputText) {
        charFreq[char] = (charFreq[char] || 0) + 1;
    }

    const root = buildHuffmanTree(charFreq);
    const codes = {};
    generateCodes(root, '', codes);

    displayFrequencyTable(charFreq);
    displayHuffmanTree(root);
    displayEncodedText(inputText, codes);
    displayDecodedText(inputText, codes, root);
}

function displayFrequencyTable(charFreq) {
    const table = document.getElementById('frequencyTable');
    table.innerHTML = '<h3>Frequency Table</h3>';
    for (let char in charFreq) {
        table.innerHTML += `${char}: ${charFreq[char]}<br>`;
    }
}

function displayHuffmanTree(root) {
    const tree = document.getElementById('huffmanTree');
    tree.innerHTML = '<h3>Huffman Tree</h3>';
    const traverse = (node, indent = '') => {
        if (!node) return;
        tree.innerHTML += `${indent}${node.character || '*'}: ${node.freq}<br>`;
        if (node.left || node.right) {
            traverse(node.left, indent + '  ');
            traverse(node.right, indent + '  ');
        }
    };
    traverse(root);
}

function displayEncodedText(inputText, codes) {
    const encoded = document.getElementById('encodedText');
    encoded.innerHTML = '<h3>Encoded Text</h3>';
    let encodedStr = '';
    for (let char of inputText) {
        encodedStr += codes[char];
    }
    encoded.innerHTML += encodedStr;
}

function displayDecodedText(inputText, codes, root) {
    const encodedText = document.getElementById('encodedText').innerText.replace('Encoded Text', '').trim();
    const decoded = document.getElementById('decodedText');
    decoded.innerHTML = '<h3>Decoded Text</h3>';
    let currentNode = root;
    let decodedStr = '';
    for (let bit of encodedText) {
        currentNode = bit === '0' ? currentNode.left : currentNode.right;
        if (currentNode.character) {
            decodedStr += currentNode.character;
            currentNode = root;
        }
    }
    decoded.innerHTML += decodedStr;
}