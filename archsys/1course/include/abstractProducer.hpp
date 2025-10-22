#ifndef ABSTRACTPRODUCER_HPP
#define ABSTRACTPRODUCER_HPP

namespace course
{
  class Producer
  {
  public:
    virtual void produceRequest() = 0;
    virtual ~Producer() = default;
  };
} // namespace course

#endif
