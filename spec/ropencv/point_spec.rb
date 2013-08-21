require 'spec_helper'

describe OpenCV::Point do
  let(:point) { OpenCV::Point.new(10, 20) }

  describe '.new' do
    it 'should require two arguments' do
      expect { OpenCV::Point.new }.to raise_error ArgumentError
      expect( point ).to be_a OpenCV::Point
    end
  end
  
  describe 'accessors' do
    describe '#x' do
      it 'should return the correct X coordinate' do
        expect( point.x ).to eq(10)
      end
    end

    describe '#y' do
      it 'should return the correct Y coordinate' do
        expect( point.y ).to eq(20)
      end
    end
  end

  describe 'setters' do
    describe '#x=' do
      it 'should set the value of the X coordinate' do
        point.x = 20
        expect( point.x ).to eq(20)
      end
    end
    describe '#y=' do
      it 'should set the value of the Y coordinate' do
        point.y = 40
        expect( point.y ).to eq(40)
      end
    end
  end
end
