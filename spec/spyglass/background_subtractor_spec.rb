require 'spec_helper'

[Spyglass::BackgroundSubtractor::MOG,
 Spyglass::BackgroundSubtractor::MOG2,
 Spyglass::BackgroundSubtractor::GMG,
 Spyglass::BackgroundSubtractor::PratiMediod].each do |algorithm|
   describe algorithm do
     let(:bg) { described_class.new }
     let(:lena) { Spyglass::Image.load(fixture_path('lena.jpg')) }

     describe '.new' do
       it 'should work with no arguments' do
         expect( bg ).to be_a described_class
       end

       it 'should work with an options hash' do
         expect { algorithm.new(history: 50, threshold: 64) }.not_to raise_error
       end
     end

     describe '#subtract' do
       it 'should return a Spyglass::Image' do
         delta = bg.subtract(lena, 1)
         expect( delta ).to be_a Spyglass::Image
       end
     end
   end
 end
