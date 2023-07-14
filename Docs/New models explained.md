# Adding ORM-models system to existing DAO & Repository


## Intro

Since ur project uses database, we needed to be able to manage storing data.  
At the time of implementation of new model's system, we were using DAO & Repository pattern. This stuff worked well and clearly, but at the same time something was left unimproved.
'Something' was our models system, which wasn't the most convenient way, but the simplest one. All that forced us to use different casts and dealing with raw types inside business logic.  
Thereby, code became unpleasurable and unclear, therefore it's difficult to support it.
In addition, all this casts and raw types must not be in business logic, they must be hidden from user. I decided that it can be fixed by creating new model system.  
Thus, new model system is created.

## Why new model system

The problem of old system was that directly models were compile-time structures with defined fields. They represent fields(columns) from DB. That's the most obvious solution ever.
Take a look of usage example (I've chosen the worst example);

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

Looks terrible, doesn't it?

> 'value' above is variable of raw return type, which is multi-dimensional array with specific interface.

However, using old models also means that we cannot make it automated. No doubt, we can do it, but let's simply look how it will be.  
Just imagine that we have 10 models, they represent 10 tables from DB. We want to erase all casts from business logic.
So, this means that we need to write 10 overrided methods which will do the cast work. Cool, but what if we need to cast only a part of it? How to define what fields we want to cast?
Of course, it also means that we have to write a lot of similar code inside this overridings.

Resulting, we have next:
- Old system cannot be automated
- It's difficult to support it
- It's difficult to expand this system
- User must know how to work with raw return types 

It means that's going to be too expensive to have old system in the future.

All of this led to the new system that should be:
- Able to use cast, but it should be hidden from user
- Able to store data as a field
- Able to be expanded and supported


## Base model

The question was how to hold data if we're not going to have any fields like fields in old models. Plus, it's easy to understand that there will be a lot of calls of fields.
Like situation, when user wants to initialize a field by any incoming data. That led my mind to use a container as a data storage. I've chosen a map for this purpose, because it's cheaper to use than a vector when user wants to get access to the field.
Therefore, we need to use a key for the map. Using a __size_t__ or __std::string/string_view__ isn't a best option there.
User must know names of fields, that isn't convinient way. So, I've chosen an enum that includes names of fields and at the same time it can be a key for the map.    
Quite simple at the start. It's obvious that we're going to have a lot common logic inside models, therefore we need to create a kind of base class that can hold this common logic.
In addition, somehow we need to instance what kind of model it's going to be. Plus, we have to choose enum for the specific model/class. I decided that making a template parameter which will instance a specific model is the best option there.
So, I've created a base class called 'UnifiedModel' with template parameter. It's in charge of defining what model it is and at the same time it's a key for the map.  

> Also this class has a pure virtual function, so it makes the whole class abstract.

Everything that user needs to use is something that gives him access to the fields. Because, base class includes only container as a data storage, I decided to define operator[] as a main method which gives access.  
I have to mention, that's not all methods. Thanks to them all system can be automated.  

Let's look at them:

### TEnum getEnumField(size_t num) const = 0;

```c++
    template <typename TEnum>
    class UnifiedModel
    {
        ...
    protected:
        virtual TEnum getEnumField(size_t num) const = 0;
        ...
    }
```

This method is used in method 'init()' which is described below, to get enum field from size_t counter. This enum field will be a unique key for the map.


### void init(const FieldNames& fieldNames)

```c++
...
    using FieldNames = std::vector<std::string_view>;
...
template<typename TEnum>
class UnifiedModel
{
    ...
protected:
    void init(const FieldNames& fieldNames)
    {       
        for (size_t counter{ 0 }; counter < _amountOfFields; ++counter)
            _data.insert({ this->getNumEnum(counter), {fieldNames[counter], std::string{}} });
    }
    ...
}
```

This method designed to be used in derived default class constructor to fill a map with fields of enum.

- '_data' is a main storage unit - map
- The order of incoming fieldNames and order in getEnumField overriding must be the same. Otherwise, you can get a situation when you want to initialize 'email', but you do 'password'


### TEnum toEnum(std::string_view fieldName) const

```c++
template<typename TEnum>
class UnifiedModel
{
    ...
public:
    TEnum toEnum(std::string_view fieldName) const
    {
        return std::find_if(_data.cbegin(), _data.cend(), [&fieldName](const auto& pair)
                            {
                                if (pair.second.first == fieldName)
                                    return true;
                                else
                                    return false;
                            })->first;
    }
    ...
}
```

Method is designed to make automated and hidden casts possible at the PGRepository side. It converts incoming fieldName in enum field to get access to field.
This fieldName comes from pqxx as a part of answer from DB. Also, makes possible dealing 

### InsertData makeColumnDataPair() const

```c++
    using InsertData = std::vector<std::pair<std::string_view, std::string_view>>;
    ...
    template<typename TEnum>
    class UnifiedModel
    {
    ...
    public:
    InsertData makeColumnDataPair() const
    {
        InsertData pairs;
        pairs.reserve(_amountOfFields);

        std::for_each(_data.cbegin(), _data.cend(), [&pairs](const auto& pair)
                      {
                          if (!std::empty(pair.second.second))
                              pairs.emplace_back(pair.second);
                      }
        );
        return pairs;
    }
    ...
}
```

> Also, compiler uses there NRVO (named return value optimization).

This one is designed to make read-only pairs of <field_name, field_value>. Used inside DAO methods, but the best performance for user we can see inside Repository logic.  
Before, when we wanted to insert something in DB by DAO, we were creating tuple of pairs just inside business logic and that looked quite awful.  
Thus, method makes possible to transfer only model in DAO interface. Moreover, the whole stuff looks more ORM and no more tuples there.  

```c++
    _pTable->Insert()->columns(&userChannels)->execute();
```


### Mutable base class field

Eventually, I have to mention 'mutable' near main class variable.  
I tried to avoid it but it will cause more changes at more sides of project, so the best case was to make it mutable. Let me describe why.  
In any of our repositories we transfer our model as const reference. It means, that user can't change this model and therefore can't change fields. So, everything that I can do without mutable is to create a new model inside a method and copy/move data from argument model.
Also, I have to mention that not only from there we use const reference, but at server side too. There is a special method called 'fmt' which is used to make const reference. 
Creating a new model inside this repository method looks quite ugly, because we already have an object.
Other choice was to make data field mutable and define methods which are operating data as 'const', even when it's not going to be const, like 'operator[]'.

```c++
    Utility::ChannelCreateCodes createChannel(const Models::ChannelInfo& channel) override;
```

```c++
    auto futureResult = _repoManager->pushRequest(&IChannelsRepository::deleteChannel, fmt(channelDeleteInfo));
    /// channelDeleteInfo -> model that transfers as an argument in repository method
```

## Specific models

Obviously, that endpoint models inherit base class, override pure virtual method and define template argument, represent table from DB and so on.  
Also, they are totally independent from any DB. It can be used everywhere.  
The usage of them is similar with previous models. In general we have the same fields, but access there we get by overrided ***operator[]***, not by ***operator.***, which is cannot be overrided.
The internal realization is container and therefore we can use container advantages when we need to fill fields by incoming data(non-DB). I mean we can create automated initialization and it has done.  
Eventually, for user it's the same model, but with different field access, but other entities may use it as a container.

> ***_data**** is a main data storing field.

```c++
    TEnum toEnum(std::string_view fieldName) const
    {
        return std::find_if(_data.cbegin(), _data.cend(), [&fieldName](const auto& pair)
                            {
                                if (pair.second.first == fieldName)
                                    return true;
                                else
                                    return false;
                            })->first;
    }
```

## Filling models

At the begginning we've wanted to build Data-Mapper pattern. It claims that models are in charge of filling themselves. Simultaneously, it causes a lot of problems.  
Main, but not the most difficult was single responsibility problem. I mean that class was filling and storing data and that's not so good. Only one task must stay.
Also, it declares that base abstract class must have 2 template arguments, but we need to know them at different steps of usage.
I mean that we need to know by what type we want to fill and probably we don't know when model uses specific filling type, but we always have to know another one, which describes the model by itself.  
It led to quite strange structure - template inheritance.

```c++
    template<template<typename ...TArgs> class RepositoryType, class TEnum = UserInfo, typename... Args>
    class User : public RepositoryType<TEnum, Args...>
    {
        ...
    };
```

There *RepositoryType* was a class, which was directly inherited base class and also this class implemented filling logic.  
However, it didn't work well with other project stuff that's why I decided to reject it and think about simpler solution.
I moved filling logic to another class 'ModelFiller'. I wanted to create it polymorphic, but I faced the same issue - I have to know two template params at different moments of time.
It can be easily solved if it may be possible to create template pure virtual method which will be overrided in derived, but c++ doesn't allow it.  
So, I just put this stuff into one class named PGFiller(which includes filling logic with return type from pqxx library) and made this class as a field of abstract class of PG Repository.
Like it was made to PGQuery (PG query builder).
