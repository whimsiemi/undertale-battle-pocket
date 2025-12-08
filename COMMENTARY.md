# FGCT4015 Fundamentals of Games Develop

## UNDERTALE: Pocket Battle

Name: Emily Thomas

Date: 05/12/2025

[Repository Link](https://github.com/whimsiemi/undertale-battle-pocket)

[Build Link](https://github.com/whimsiemi/undertale-battle-pocket/releases/tag/final)

[Demo Video]([https://www.youtube.com/watch?v=oQw88WtnwII])

### Project Outline

- A remake of the RPG battle system from UNDERTALE, made within Raylib and styled within the base limitations of the Nintendo Game Boy.

### Research

#### Methodology

- I'm largely took direct reference from UNDERTALE's gameplay footage and artwork, figuring out the fundamental mechanics behind the battle system and piecing together the process for making it all work by reading up on Raylib's documentation

#### Game Sources

- UNDERTALE (Toby Fox, 2015): [Read more about the game here](https://store.steampowered.com/app/391540/Undertale/)

#### Academic Sources

- I'm going to be honest: I didn't really find any academic sources throughout my research that actually aided the creation of this project, with it being such a direct translation that its input wouldn't be heavily necessary

#### Documentation Sources

- Raylib examples page by Raylib Technologies: https://www.raylib.com/examples.html
- "An Introduction to Raylib" by Coding with Sphere: https://www.youtube.com/watch?v=AniAoJC6QAc

### Implementation

#### Process

- 1: I would write out the feature/fix's criteria in detail (for example: the Attack option required a health system as well as the accuracy microgame)
- 2: I would piece together which examples would help me and piece together the libraries and overall program flow that would be applicable to my feature/fix, as well as search around for Stack Overflow threads if I needed specific advice not showcased within these examples
- 3: I would begin a process of trial and error, cross referencing my sources and debugging each criteria individually until they were fully functional (repeating step 2 if the information I have isn't enough)

#### New Approaches

- This was my first time using Raylib
-- The aforementioned examples were crucial to understanding practical usage of the engine, as well as some more niche syntaxes
- I had to make Game Boy styled music (albeit a remix) for the first time
-- This involved learning Trackerboy, a Game Boy sound chip music tracker software that involves inputting each played note into a top-to-bottom manner, with variables (volume, FX, etc) being read as hexadecimal values
-- My prior background in music composition made it relatively easy to transcribe the original battle song in a way that sounded good on the Game Boy's sound chip, but it was far more time consuming than other composition workflows

#### User Testing

- I did user testing about 2 weeks into development, when the attacking and dodging mechanics had just been implemented
- If I'm being honest, most of the feedback was relatively unhelpful, as it would be reaffirming things that I had already disclosed to the testers beforehand (i.e. the lack of a mercy system, which I mentioned serveral times was being worked on)

![Data](resources/form%20responses.png)

#### Instructions To Install/Run

- 1: Download the .zip file on the build page
- 2: Extract the .zip file into its own folder
- 3: Double click the .exe file

### Reflection

#### Research Effectiveness

- The research I did was essential to the project, as I had to go from knowing little to nothing about Raylib and C++ to making a fully-fledged demo within 10 weeks and the sources that I found proved incredibly helpful

#### Positive Analysis

- The visual style remained relatively consistent
- The gameplay was intuitive to playtesters

#### Negative Analysis

- I worked myself so hard that I got burnt out incredibly quickly and struggled to work on other projects after this, including an Unreal Engine game made for the same assignment
- The codebase is a pretty big mess, as I was rushing to get features implemented without a lot of care putting into codebase maintenance

#### Next Time

- I need to take more regular breaks from work, unless I want to burn myself out again
- I need to slow myself down in regards to feature implementation, taking more time to maintain a concise codebase

### Declared Assets

- All assets made for this game were made by me, but sprites from UNDERTALE (largely drawn by Toby Fox and Temmie Chang) were heavily used as reference

### References

UNDERTALE (2015) At: https://store.steampowered.com/app/391540/Undertale/

