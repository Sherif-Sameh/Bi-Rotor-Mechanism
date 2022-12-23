function [result] = reconstructNumber(highByte,lowByte)
% Takes in the low and high bytes of a number and reconstructs the orginal
% number.

result = int16(highByte)*power(2,8) + int16(lowByte);
end

