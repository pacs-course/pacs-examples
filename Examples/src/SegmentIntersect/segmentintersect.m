function [P,t,status]=segmentintersect(S1,S2,tol,len1,len2)
%function [P,t,status]=segmentintersect(S1,S2,tol,len1,len2)
%
% Given the two segments 
%  S1=[x1_A;y1_A;x1_B;y1_B]
% and
%  S2=[x2_A;y2_A;x2_B;y2_B] (see figure)
% This function finds the intersection  P=[x;y] between 
% their lines and the parameter values t1=t(1) and t2=t(2)
% in the following parametric representation of the two
% lines:
%
%    { x(t1)=(x1_B-x1_A)*t1+x1_A
% r1:{
%    { y(t1)=(y1_B-y1_A)*t1+y1_A
%
%    { x(t2)=(x2_B-x2_A)*t2+x2_A
% r2:{
%    { y(t2)=(y2_B-y2_A)*t2+y2_A
% 
%
%  If 0<=t(1)<=1 then P is internal to the segment S1,
%  if 0<=t(2)<=1 then P is internal to the segment S2.
%
%                   S1
%  (x1_A,y1_A)-------------(x1_B,y1_B)
%     t(1)=0                  t(1)=1
%
%                   S2
%  (x2_A,y2_A)-------------(x2_B,y2_B)
%     t(2)=0                  t(2)=1
%
% If the two lines are parallel or coincident then P=[Inf;Inf],t=[Inf;Inf]
% is returned
%
%  tol absolute tolerance. Two segments do not intersect if their
%      distance is >tol
%  len1 len1 optional segment length
%  status  0 no intersection
%          1 intersection
%
%  The algoritm modified by L. Formaggia works as follows
%  1) The search of intersection points is now limited to the point not
%  exterior to the segments, by clipping the parameter t between 0 and 1
%  2) Intersection at the ends of both segments are considered not
%  intersections (as in the previous version, where the check was done
%  outside segmentintersect)
%  3) If the intersection happens at one of the segment ends the basic
%  algoritms is replaced by projection: we look at the projection of the
%  segment end on the other segment.
%
%Giovanni Scrofani, 17/04/2005
%Giovanni Scrofani, 28/10/2006 (minor changes)
%Giovanni Scrofani, September 9 2009: Added a nondimensionalization to
%                                     improve accuracy and debug check
%Luca Formaggia, April 2011 Changed the algorithm
S1=S1(:);
S2=S2(:);
status=1;
if nargin <=3
  len1=sqrt((S1(1)-S1(3))*(S1(1)-S1(3))+(S1(2)-S1(4))*(S1(2)-S1(4)));
  len2=sqrt((S2(1)-S2(3))*(S2(1)-S2(3))+(S2(2)-S2(4))*(S2(2)-S2(4)));
end

A=[S1(3:4)-S1(1:2),S2(1:2)-S2(3:4)];
D=A(1,1)*A(2,2)-A(1,2)*A(2,1);
if (abs(D)<=10*eps*len1*len2)
% Parallel segments
    P=[Inf;Inf];
    t=[Inf;Inf];
    status=0;
    return;
end
t_tol=zeros(1,2);
t_tol(1)=tol/len1;
t_tol(2)=tol/len2;
t=A\(S2(1:2)-S1(1:2)); 
% The following condition is true when two segments
%  intersecate each other at their endpoints 
if (( (abs(t(1)) <= t_tol(1)) || (abs(t(1)-1) <= t_tol(1)) ) &&...
    ( (abs(t(2)) <= t_tol(2)) || (abs(t(2)-1) <= t_tol(2)) ))
    status=0;    
    P=[Inf;Inf];
    t=[Inf;Inf];
    return;
end
% clip to segment ends
tclip=min(max(t,0),1);
if (tclip(1)==0 || tclip(1) == 1)
    % clipped seg 1 -> projection
    P       =(S1(3:4)-S1(1:2))*tclip(1)+S1(1:2);
    tclip(2)=dot(P(1:2)-S2(1:2),S2(3:4)-S2(1:2))/len2/len2;
    tclip(2)=min(max(tclip(2),0),1);
    if (tclip(2)==0 || tclip(2) ==1) % intersection at ends
        status=0;
        t=tclip;
        return;
    end
    Pcheck  =(S2(3:4)-S2(1:2))*tclip(2)+S2(1:2);
elseif (tclip(2)==0 || tclip(2) == 1)
    % clipped seg 2 -> projection
    P       =(S2(3:4)-S2(1:2))*tclip(2)+S2(1:2);
    tclip(1)=dot(P(1:2)-S1(1:2),S1(3:4)-S1(1:2))/len1/len1;
    tclip(1)=min(max(tclip(1),0),1);
    if (tclip(1)==0 || tclip(1) ==1) %intersection at ends
        status=0;
        t=tclip;
        return;
    end
    Pcheck =(S1(3:4)-S1(1:2))*tclip(1)+S1(1:2);
else
    P      =(S1(3:4)-S1(1:2))*tclip(1)+S1(1:2);
    Pcheck =(S2(3:4)-S2(1:2))*tclip(2)+S2(1:2);
end
if (norm(P-Pcheck,'inf')>2*tol)
    status=0;
    P=[Inf;Inf];
    t=[Inf;Inf];
else
    t=tclip;
    status=1;
end
return