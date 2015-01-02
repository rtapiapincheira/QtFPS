close all;
clear all;
clc;

T = load('output2.txt');
U = mat2gray(T);
imshow(U);
