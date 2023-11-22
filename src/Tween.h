#pragma once

namespace Tween
{
    enum TweenType
    {
        Linear,

        Sine_EaseIn,
        Sine_EaseOut,
        Sine_EaseInOut,

        Quad_EaseIn,
        Quad_EaseOut,
        Quad_EaseInOut,

        Cubic_EaseIn,
        Cubic_EaseOut,
        Cubic_EaseInOut,

        Quart_EaseIn,
        Quart_EaseOut,
        Quart_EaseInOut,

        Quint_EaseIn,
        Quint_EaseOut,
        Quint_EaseInOut,

        Expo_EaseIn,
        Expo_EaseOut,
        Expo_EaseInOut,

        Circ_EaseIn,
        Circ_EaseOut,
        Circ_EaseInOut,

        Elastic_EaseIn,
        Elastic_EaseOut,
        Elastic_EaseInOut,

        Back_EaseIn,
        Back_EaseOut,
        Back_EaseInOut,

        Bounce_EaseIn,
        Bounce_EaseOut,
        Bounce_EaseInOut,
    };

    float easeIn(float time, float rate);

    float easeOut(float time, float rate);

    float easeInOut(float time, float rate);

    float bezieratFunction(float a, float b, float c, float d, float t);

    float quadraticIn(float time);

    float quadraticOut(float time);

    float quadraticInOut(float time);

    float quadraticInOut(float time);

    float tweenTo(float time, TweenType type, float* easingParam);

    float linear(float time);

    float sineEaseIn(float time);

    float sineEaseOut(float time);

    float sineEaseInOut(float time);

    float quadEaseIn(float time);

    float quadEaseOut(float time);

    float quadEaseInOut(float time);

    float cubicEaseIn(float time);

    float cubicEaseOut(float time);

    float cubicEaseInOut(float time);

    float quartEaseIn(float time);

    float quartEaseOut(float time);

    float quartEaseInOut(float time);

    float quintEaseIn(float time);

    float quintEaseOut(float time);

    float quintEaseInOut(float time);

    float expoEaseIn(float time);

    float expoEaseOut(float time);

    float expoEaseInOut(float time);

    float circEaseIn(float time);

    float circEaseOut(float time);

    float circEaseInOut(float time);

    float elasticEaseIn(float time, float period);

    float elasticEaseOut(float time, float period);

    float elasticEaseInOut(float time, float period);

    float backEaseIn(float time);

    float backEaseOut(float time);

    float backEaseInOut(float time);

    float bounceEaseIn(float time);

    float bounceEaseOut(float time);

    float bounceEaseInOut(float time);
}