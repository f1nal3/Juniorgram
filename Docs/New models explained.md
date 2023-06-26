# Adding to DAO & Repository ORM-models

## Intro

We've created a strucure of Data access object(DAO) and it worked. However, at the same time it created a problem.
Inside business logic we're having a lot of casts and references. In general, it's almost impossible to understand them from day one. 
Also, this code is too dirty and includes what I mentioned, a lot of casts, working with raw return type and so on, therefore it's difficult to support it.
It must not be in business, most of problems where at the side of models. They are the problem that causes us to use this dirty stuff.
Thus, new models are created

## New models

The problem of old ones was their fields. Old model was a structure with defined fields, they are mapping fields(columns) from DB. That's the most obvious solution ever.
Take a look how it is (I've chosen the worst example)

```c++
    messageInfo._msgID = value[0].as<std::uint64_t>();
    messageInfo._senderID = value[1].as<std::uint64_t>();
    messageInfo._time  = static_cast<std::uint64_t>(value[2].as<double>());
    messageInfo._message  = value[3].as<std::string>();
    messageInfo._userLogin = value[4].as<std::string>();
    messageInfo._reactions[0] = value[6].as<std::uint32_t>();
    messageInfo._reactions[1] = value[7].as<std::uint32_t>();
    messageInfo._reactions[2] = value[8].as<std::uint32_t>();
    messageInfo._reactions[3] = value[9].as<std::uint32_t>();
    messageInfo._reactions[4] = value[10].as<std::uint32_t>();
```

Looks terrible, isn't it?

However, this solution also means that we cannot make it automated. No doubt, we can do it, but let's simply look how it will be.
Just imagine that we have 10 models, they represent 10 tables from DB. Perfect, let's move to the next point. We want to erase all casts from business logic.
This means that we need to write 10 overrided methods which will do the cast work. Cool, but what if we need to cast only a part of it? How to define what fields we want to cast?
Of course, it also means that we have to right a lot of similar code inside this overridings

It means that's going to be too expensive to make them, and even more expensive if we need to support and expand this stuff

It led to the new system that should be:
- Able to use cast 
- Able to store fields
- Able to be expanded and supported

The question was how to hold data if we're not going to have any fields like fields in old models. Also, it's easy to understand that there will be a lot of field calling.
I mean, like we want to initialize a field and there're a lot of this. That's why I've chosen a map because it's cheaper to use than a vector when we call a field.
If that's a map, we need use a key for this map. Using a __size_t__ or __std::string/string_view__ isn't a best option there.
Endpoint user must know names of fields, that isn't convinient way. So, we've chosen an enum class that includes names of fields and at the same time it can be a key for the map.
Quite simple at the start. It's obvious that we're going to have a lot common logic inside models, therefore we need to create a kind of base class that can hold this common logic.
It's called UnifiedModel. Also, it has a template parameter which is enum class for specific model(table). 

> Also this class has a pure virtual function, so it makes the whole class abstract

This class consists of some non-common methods, let's look at them

```c++
    template <typename TEnum> /// Method isn't a template one, only class, this template used there to underline usage of template parameter
    virtual TEnum getNumEnum(size_t num) const = 0;
```

This class is needed to fill the map with the same key with 'fields'.
They are represented as fields of enum class. It can be compared with creating a list of candies of different colors, but by the same producer (like Roshen)
Thus, we always know what candy do we need right now. But at this level we don't know anything about candies so this method must be overrided by derived class

```c++
    void init(const FieldNames& fieldNames)
    {       
        for (size_t counter{ 0 }; counter < _amountOfFields; ++counter)
            _data.insert({ this->getNumEnum(counter), {fieldNames[counter], std::string{}} });
    }
```

This method used in derived default class constructor to fill a map with incoming specification

> The order of incoming fieldNames and order in getNumEnum overriding must be the same. Otherwise, you can get a situation when you want to initialize 'email', but you do 'password'

```c++
    TEnum toEnum(std::string_view fieldName) const
```

Method is designed to make filling magic possible at the PGRepository side. It converts incoming fieldName in enum field to get access to field. 
This fieldName comes from pqxx as a part of returning of SQL query. Also, makes possible dealing with DAO by model.

```c++
    InsertData makeColumnDataPair() const
```

Eventually, I have to mention 'mutable' near main class variable.
I tried to avoid it but it will cause more changes at more sides of project, so the best case was to make it mutable. Let me describe why.
In any of our repository we transfer our model as const reference, so everything that I can do without mutable is to create a new model inside a method and copy/move data from argument model.
Also, I have to mention that not only from there we use const reference, but at server side too. There is a special method called 'fmt' which is used to make const reference. 
Ñreating a new model inside this repository method looks quite ugly, because we already have an object.
Other choice was to make data field mutable and define methods which are operating data as 'const', even when it's not going to be const, like 'operator[]'

```c++
    Utility::ChannelCreateCodes createChannel(const Models::ChannelInfo& channel) override;
```

```c++
    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));
    /// channelDeleteInfo -> model that transfers as an argument in repository method
```


## Filling models

At the begginning we've wanted to build Data-Mapper pattern. It claims that models are in charge of filling themselves. Simultaneously, it causes a lot of problems.
Main, but not the most difficult was single responsibility problem. It means that class was filling and storing data.
Also, it declares that base abstract class must have 2 template arguments, but we need to know them at different steps of usage.
I mean that we need to know by what type we want to fill and probably we don't know when model uses specific filling type, but we always have to know another one, which describes the model by itself.
It led to quite strange structure - template inheritance.

```c++
    template<template<typename ...TArgs> class RepositoryType, class TEnum = UserInfo, typename... Args>
    class User : public RepositoryType<TEnum, Args...>
    {
        ... class stuff
    };
```

There RepositoryType was a class, which was directly inherited base class and also this class implemented filling logic.
However, it didn't work well with another project stuff that's why I decided to reject it and think about simpler solution.
I moved filling logic to another class 'ModelFiller'. I wanted to create it polymorphic, but I faced the same issue - I have to know two template params at different moments of time.
It can be easily solved if it may be possible to create template pure virtual method which will be overrided in derived, but c++ doesn't allow it.
So, I just put this stuff into one class named PGFiller(which includes filling logic with return type from pqxx library) and made this class as a field of abstract class of PG Repository.
Like it was made to PGQuery (PG query builder).
