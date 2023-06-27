## Introduction

IRepository is an interface for a more abstract approach to Databases.

## Reasoning

We need a constant way to access any data storage that we might use.
So far the use cases are:
* PostgreSQL for storing data on the kernel side
* MySQLite for storing data on the client side
* File Database Mock used for debug purposes

Since we mirror the way the data is stored - all of those points can share the same interface.

## Description
IRepository dictates all possible cases for storing or getting data.
Each counterpart described above implements IRepository via corresponding adapters.

## Schema for PostgreSQL case

![Basic Repository schema](https://i.ibb.co/54yzrzw/Screenshot-2021-03-15-at-23-49-42.png)