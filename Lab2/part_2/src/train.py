#!/usr/bin/env python3
import torch
import torch.nn as nn
import torch.nn.functional as F
from typing import List
import math


class char_tokenizer:
    """
    a very simple char-based tokenizer. the tokenizer turns a string into a list of integers.
    """

    def __init__(self, corpus: List[str]):
        self.corpus = corpus
        # TODO: calculate the vocab size and create a dictionary that maps each character to a unique integer
        self.char_to_int = {char: i for i, char in enumerate(self.corpus)}
        self.int_to_char = {i: char for i, char in enumerate(self.corpus)}
        self.n_vocab = len(self.char_to_int)
        # End of your code

    def encode(self, string: str):
        # TODO: convert a string into a list of integers and return, using the dictionary you created above
        return [self.char_to_int[char] for char in string]
        # End of your code

    def decode(self, codes: List[int]):
        # TODO: convert a list of integers into a string and return, using the dictionary you created above
        return "".join(self.int_to_char[code] for code in codes)
        # End of your code


class Head(nn.Module):
    """single head of self-attention"""

    def __init__(self, head_size):
        super().__init__()
        # TODO: create three linear layers, Key, Query, and Value, each of which maps from n_embd to head_size
        #       and assign them to self.Key, self.Query, and self.Value, respectively
        self.Key = nn.Linear(n_embd, head_size)
        self.Query = nn.Linear(n_embd, head_size)
        self.Value = nn.Linear(n_embd, head_size)
        # End of your code
        self.register_buffer("tril", torch.tril(torch.ones(block_size, block_size)))

    def forward(self, inputs):
        # TODO: implement the forward function of the head
        #       the input is a tensor of shape (batch, time, n_embd)
        #       the output should be a tensor of shape (batch, time, head_size)
        #       you may use the tril buffer defined above to mask out the upper triangular part of the affinity matrix
        key = self.Key(inputs)
        query = self.Query(inputs)
        value = self.Value(inputs)
        attn_scores = torch.matmul(query, key.transpose(-2, -1)) / math.sqrt(key.size(-1))
        attn_scores = attn_scores.masked_fill(~self.tril[:attn_scores.size(-2), :attn_scores.size(-1)].bool(),
                                              float('-inf'))
        attn_weights = F.softmax(attn_scores, dim=-1)
        out = torch.matmul(attn_weights, value)
        # End of your code
        return out


class MultiHeadAttention(nn.Module):
    def __init__(self, n_heads, head_size):
        super().__init__()
        # TODO: implement heads and projection
        self.heads = nn.ModuleList([Head(head_size) for _ in range(n_heads)])
        self.projection = nn.Linear(n_heads * head_size, n_embd)

    # End of your code

    def forward(self, inputs):
        # TODO: implement the forward function of the multi-head attention
        out = torch.cat([head(inputs) for head in self.heads], dim=-1)
        return self.projection(out)


class FeedForward(nn.Module):
    def __init__(self, n_embd):
        super().__init__()
        # TODO: implement the feed-forward network
        self.net = nn.Sequential(
            nn.Linear(n_embd, 4 * n_embd),
            nn.ReLU(),
            nn.Linear(4 * n_embd, n_embd),
        )
        # End of your code

    def forward(self, inputs):
        return self.net(inputs)


class Block(nn.Module):
    def __init__(self, n_embd, n_heads):
        super().__init__()
        # TODO: implement the block of transformer using the MultiHeadAttention and
        # FeedForward modules, along with the layer normalization layers
        self.ln1 = nn.LayerNorm(n_embd)
        self.attn = MultiHeadAttention(n_heads, n_embd // n_heads)
        self.ln2 = nn.LayerNorm(n_embd)
        self.mlp = FeedForward(n_embd)
        # End of your code

    def forward(self, inputs):
        # TODO: implement the forward function of the block, you may refer to the docs of this experiment
        x = self.attn(inputs)
        x = self.ln1(inputs + x)  # Add & Norm
        y = self.mlp(x)
        y = self.ln2(y + x)  # Add & Norm
        return y
        # End of your code


class Transformer(nn.Module):
    def __init__(self):
        super().__init__()
        # TODO: create the embedding table, the stack of blocks, the layer normalization layer,
        # and the linear layers.
        self.embedding = nn.Embedding(n_vocab, n_embd)
        self.blocks = nn.ModuleList([Block(n_embd, n_heads) for _ in range(n_layers)])
        self.ln = nn.LayerNorm(n_embd)
        self.fc = nn.Linear(n_embd, n_vocab)
        # End of your code

    def forward(self, inputs, labels=None):
        # TODO: implement the forward function of the transformer
        x = self.embedding(inputs)
        for block in self.blocks:
            x = block(x)
        x = self.ln(x)
        logits = self.fc(x)
        if labels is None:
            loss = None
        else:
            batch, time, channel = logits.shape
            logits = logits.view(batch * time, channel)
            labels = labels.view(batch * time)
            loss = F.cross_entropy(logits, labels)
        return logits, loss

    def generate(self, inputs, max_new_tokens):
        # TODO: generate new tokens from the transformer, using the inputs as the context,
        #  and return the generated tokens with length of max_new_tokens
        inputs = inputs.clone()
        for _ in range(max_new_tokens):
            # generates new tokens by iteratively sampling from the model's predicted probability distribution,
            # concatenating the sampled tokens to the input sequence, and returning the updated sequence.
            # logits, _ = self(inputs)
            # _, max_indices = torch.max(logits, dim=-1)
            # new_token = max_indices[:, -1:]
            # inputs = torch.cat([inputs, new_token], dim=1)
            inputs_within_block_size = inputs[:, -block_size:]
            logits, _ = self(inputs_within_block_size)
            probs = F.softmax(logits[:, -1, :], dim=-1)
            new_token = torch.multinomial(probs, num_samples=1)
            inputs = torch.cat([inputs, new_token], dim=-1)
        # End of your code
        return inputs


def get_batch(split):
    data = train_data if split == "train" else val_data
    ix = torch.randint(len(data) - block_size, (batch_size,))
    x = torch.stack([data[i: i + block_size] for i in ix])
    y = torch.stack([data[i + 1: i + block_size + 1] for i in ix])
    x, y = x.to(device), y.to(device)
    return x, y


@torch.no_grad()
def estimate_loss(model):
    out = {}
    model.eval()
    for split in ["train", "val"]:
        losses = torch.zeros(eval_iters)
        for k in range(eval_iters):
            x, y = get_batch(split)
            logits, loss = model(x, y)
            losses[k] = loss.item()
        out[split] = losses.mean()
    return out


def generate(model):
    context = torch.zeros((1, 1), device=device, dtype=torch.long)
    print(decode(model.generate(context, max_new_tokens=500)[0].tolist()))


def train(model):
    optimizer = torch.optim.AdamW(model.parameters(), lr=learning_rate)

    for iter in range(max_iters):

        if iter % eval_interval == 0:
            losses = estimate_loss(model)
            print(
                f"step {iter}: train loss {losses['train']:.4f}, val loss {losses['val']:.4f}"
            )

        inputs, labels = get_batch("train")

        logits, loss = model(inputs, labels)
        optimizer.zero_grad(set_to_none=True)
        loss.backward()
        optimizer.step()


# define the hyperparameters
batch_size = 16
block_size = 256
max_iters = 50  # set the number of training iterations as you like
eval_interval = 50
learning_rate = 1e-3
device = "cuda" if torch.cuda.is_available() else "cpu"
eval_iters = 200
n_embd = 64
n_heads = 8
n_layers = 6

# read the dataset
with open("../data/input.txt", "r", encoding="utf-8") as f:
    text = f.read()
chars = sorted(list(set(text)))

# initialize the vocabulary
tokenizer = char_tokenizer(chars)
encode = tokenizer.encode
decode = tokenizer.decode
n_vocab = tokenizer.n_vocab

# separate the dataset into train and validation
train_data = torch.tensor(encode(text[: -len(text) // 10]), dtype=torch.long)
val_data = torch.tensor(encode(text[-len(text) // 10:]), dtype=torch.long)

# define the model
model = Transformer().to(device)
train(model)
generate(model)
